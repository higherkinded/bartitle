#include "battery.h"
#include <stdio.h>

#define freadguard(fp, filepath) \
	guard (fp, fopen(filepath, "r")) { \
		printf("Failed to open '%s'", filepath); \
		return; \
	}

static uint battery_flags = 0;
static char type_buf[4] = "";
static uint tick = 0;

static FILE *max_fp;
static FILE *cur_fp;
static FILE *stat_fp;

void init_battery_state() {
	FILE *type_fp;
	freadguard(type_fp, _SYSFS_BAT_TYPEFILE);

	memset(type_buf, 0, sizeof(type_buf));
	(void) fscanf(type_fp, "%s", type_buf);
	fclose(type_fp);

	if (!!strncmp(_BATTERY_TYPE, type_buf, 4)) {
		printf(
			"'%s' is not a battery (type mismatch)",
			_BATTERY_TYPE
		);
		return;
	}

	set(battery_flags, FOUND_BATTERY);

	freadguard(max_fp, _SYSFS_BAT_MAXFILE);
	freadguard(cur_fp, _SYSFS_BAT_CURFILE);
	freadguard(stat_fp, _SYSFS_BAT_STATFILE);

	// Resources are acquired, all is well
	set(battery_flags, HAS_RESOURCES);

	// Can poll
	set(battery_flags, KEEP_POLLING);
}

void release_battery_resources() {
	ifset (battery_flags, HAS_RESOURCES) {
		if (max_fp) fclose(max_fp);
		if (cur_fp) fclose(cur_fp);
		if (stat_fp) fclose(stat_fp);

		unset(battery_flags, HAS_RESOURCES);
	}
}

static inline uint get_perc(ulong cur, ulong max) {
	ulong approx_cur = (cur / 100) * 100;
	return approx_cur / (max / 100);
}

static inline char *format_indicator(uint perc) {
	static char indicator[_BAT_INDICATOR_LEN];
	size_t times = _BAT_SECTION_COUNT;

	// It's just visually strange to do a full bar exclusively on 100%
	ulong fill = perc >= _BAT_FULL_THRESHOLD
	       	? _BAT_SECTION_COUNT
		: perc * _BAT_SECTION_COUNT / 100;

	memset(indicator, 0, _BAT_INDICATOR_LEN);
	times = _BAT_SECTION_COUNT;

	// Render the bar
	while (times--) strcat(
		indicator,
	       	fill > times ? _BAT_SECTION_FULL : _BAT_SECTION_EMPTY
	);

	return indicator;
}

char *format_status() {
	char status[5] = "NONE";

	rewind(stat_fp);
	(void) fscanf(stat_fp, "%4s", status);
	fflush(stat_fp);

	// If "Full" or "Charging", it must be plugged in.
	if (!strncmp(_BATTERY_CHARGING, status, 4)) return _BAT_ST_CHARGING;
	if (!strncmp(_BATTERY_FULL, status, 4))     return _BAT_ST_CHARGING;

	// If not, it's probably not plugged in.
	return _BAT_ST_DISCHARGING;
}

char *format_battery() {
	static char result[100] = "";
	ulong max;
	ulong cur;
	uint perc;

	ifunset (battery_flags, FOUND_BATTERY) return _BAT_NOT_FOUND;
	ifunset (battery_flags, KEEP_POLLING)  return _BAT_POLL_ERR;

	// Update when main loop makes tick hit zero
	if (!tick) {
		rewind(max_fp);
		if (!fscanf(max_fp, "%zu", &max)) unset(battery_flags, KEEP_POLLING);
		fflush(max_fp);

		rewind(cur_fp);
		if (!fscanf(cur_fp, "%zu", &cur)) unset(battery_flags, KEEP_POLLING);
		fflush(max_fp);

		ifunset (battery_flags, KEEP_POLLING) return _BAT_POLL_ERR;

		perc = get_perc(cur, max);

		sprintf(
			result,
		       	_BAT_FORMAT,
		       	perc,
		       	format_indicator(perc),
			format_status()
		);
	}

	tick = (tick + 1) % _TICK_COUNT;

	return result;
}
