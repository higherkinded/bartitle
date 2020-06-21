#ifndef _BARTITLE_BATTERY_H_
#define _BARTITLE_BATTERY_H_

#include "common.h"

/* If you're in a desperate need to remove the red squiggly lines you get from
 * `clangd` or similar tools due to macro expansion, undo the screw right here.
 *
 *                  [UNSCREW AT YOUR OWN RISK!]
 *                               |
 *                               V                                           */
/* BEGIN SUPPRESS_CLANGD_ERRORS *T/
#pragma message "[DEVELOPMENT MODE]: BATTERY may be undefined!"
#ifndef BATTERY
#pragma message "[DEVELOPMENT MODE]: BATTERY is undefined!"
#define BATTERY ""
#endif
// END SUPPRESS_CLANGD_ERRORS */
#ifndef BATTERY
#error "Battery name isn't provided in BATTERY flag"
#endif

#define _SYSFS_BATTERY_BASE "/sys/class/power_supply/"
#define _SYSFS_BATTERY_DIR  _SYSFS_BATTERY_BASE BATTERY "/"
#define _BATTERY_TYPE       "Battery"
#define _BATTERY_CHARGING   "Charging"
#define _BATTERY_FULL       "Full"
#define _SYSFS_BAT_TYPEFILE _SYSFS_BATTERY_DIR "type"
#define _SYSFS_BAT_CURFILE  _SYSFS_BATTERY_DIR "energy_now"
#define _SYSFS_BAT_MAXFILE  _SYSFS_BATTERY_DIR "energy_full"
#define _SYSFS_BAT_STATFILE _SYSFS_BATTERY_DIR "status"

#define _BAT_NOT_FOUND "BATTERY NOT FOUND"
#define _BAT_POLL_ERR  "BATTERY POLLING FAILURE"

// Formats
#define _BAT_FORMAT           " %03d%% %s %s"
#define _BAT_ST_DISCHARGING   "◇"
#define _BAT_ST_CHARGING      "◈"
#define _BAT_SECTION_EMPTY    "▱"
#define _BAT_SECTION_FULL     "▰"
#define _BAT_SECTION_BYTE_LEN 3
#define _BAT_SECTION_COUNT    4
#define _BAT_INDICATOR_LEN    _BAT_SECTION_BYTE_LEN * _BAT_SECTION_COUNT + 1
#define _BAT_FULL_THRESHOLD   95

// How much cycles must pass between battery checks
#define _TICK_COUNT 5

void init_battery_state();
void release_battery_resources();
char *format_battery();

enum BATTERY_FLAGS {
	FOUND_BATTERY = 0, // A power_supply with type "Battery" was found.
	KEEP_POLLING,      // Keep polling the battery files for status.
	HAS_RESOURCES,     // Handles are still open.
};

#endif /* _BARTITLE_BATTERY_H_ */
