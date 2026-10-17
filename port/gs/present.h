#ifndef PORT_GS_PRESENT_H
#define PORT_GS_PRESENT_H
#include "gs.h"
void gs_vsync(void);
void gs_window_apply(void);           /* DHD_WINDOW / _WIDTH / _HEIGHT changed: apply now (EE thread) */
void gs_present_options_apply(void);  /* DHD_PRESENT_FILTER / DHD_ASPECT changed */
extern volatile uint32_t gs_presented_frames;
/* Online races (port/hle/online.c): the window's title shows the lobby (UTF-8), and a wait for
   the other players keeps the window responsive. Both do nothing before the window exists. */
void gs_window_title(const char *utf8);
void gs_pump_messages(void);
void gs_window_close(void);  /* as the window's close button: the game ends at the next field */
/* The native renderer shows player gs_native_view's quarter of a 3-4 player split screen,
   scaled to the window (1-4; 0: the whole picture). online.c sets it during races. */
extern volatile int gs_native_view;
/* Decodes the selected display circuit into RGBA8; returns zero while disabled. */
int gs_display(Gs *g, uint32_t *pixels, unsigned *width, unsigned *height);
#endif
