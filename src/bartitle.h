#ifndef _BARTITLE_H_
#define _BARTITLE_H_

#define _DEFAULT_SOURCE

#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "common.h"

// Battery support
#ifdef _BATTERY_SUPPORT
#include "battery.h"
#endif /* _BATTERY_SUPPORT */

#define FORMAT_SIZE  15
#define BUFFER_SIZE  256
#define BAR_TEMPLATE "%s「モア」%s "
#define TIME_FORMAT  "%H:%M:%S"

enum ARG_FLAGS {
	DAEMONIZE = 0,
};

// --- Time formatting
char *format_time(void);

// --- Flags
uint parse_args(int argc, char** argv);
void process_flags(uint flags);

#endif /* _BARTITLE_H_ */
