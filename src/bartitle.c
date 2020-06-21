#include "bartitle.h"


static char title_buffer[BUFFER_SIZE];
static Display *display;
static int screen;
static Window root_window;
static volatile sig_atomic_t keep_running = 1;

static void sigint_handler(int _);
#define sigterm_handler sigint_handler // Same implementation

#ifndef _BATTERY_SUPPORT
#define init_battery_state()
#define format_battery() ""
#define release_battery_resources()
#endif /* !_BATTERY_SUPPORT */


// --- MAIN
int main(int argc, char** argv) {
	signal(SIGINT, sigint_handler);
	signal(SIGTERM, sigterm_handler);

	uint flags = parse_args(argc, argv);
	process_flags(flags);

	init_battery_state();

	while (keep_running) {
		guard (display, XOpenDisplay(NULL)) {
			fprintf(
				stderr,
				"%s: unable to open display '%s'\n",
				argv[0],
				XDisplayName(NULL)
			);
			exit(1);
		}

		screen = DefaultScreen(display);
		root_window = RootWindow(display, screen);

		sprintf(title_buffer, BAR_TEMPLATE, format_battery(), format_time());

		XStoreName(display, root_window, title_buffer);
		XCloseDisplay(display);

		usleep(250000);
	}

	release_battery_resources();

	display = XOpenDisplay(NULL);
	screen = DefaultScreen(display);
	root_window = RootWindow(display, screen);
	XStoreName(display, root_window, NULL);
	XCloseDisplay(display);
}


// --- Implementations
inline static void sigint_handler(int _) {
	(void)_;
	keep_running = 0;
}

inline char *format_time(void) {
	// Time
	time_t rawtime;
	struct tm localtime;

	// Formatted time buffer
	static char buf[FORMAT_SIZE];

	time(&rawtime);
	localtime_r(&rawtime, &localtime);

	strftime(buf, sizeof(buf), TIME_FORMAT, &localtime);

	return buf;
}

inline uint parse_args(int argc, char** argv) {
	uint flags = 0;

	for (int i = 1; i < argc; i++) {
		ifeq(argv[i], "-d") set(
				flags,
				DAEMONIZE
				);
	}

	return flags;
}

inline void process_flags(uint flags) {
	ifset(flags, DAEMONIZE) (void) daemon(0, 0);
}
