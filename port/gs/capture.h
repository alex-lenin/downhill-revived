#ifndef PORT_GS_CAPTURE_H
#define PORT_GS_CAPTURE_H
#include "gs.h"
/* Optional PCSX2-compatible capture; all output must go to an ignored build directory. */
void gs_capture_vsync(Gs *g);
/* DHD_RACE_GSDUMP=dir: capture the next 2 fields as dir/NAME.gs (a race moment, from the EE
   thread; port/hle/autopad.c). */
void gs_capture_request(const char *name);
#endif
