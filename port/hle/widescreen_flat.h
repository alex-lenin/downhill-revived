/* host_widescreen_flat_limit's rule, read from guest RAM: by the core (port/hle/widescreen.c) and by
   the browser's GS reactor (port/web/gpu_rust_web_native.inc), which shares the core's memory. */
#ifndef WIDESCREEN_FLAT_H
#define WIDESCREEN_FLAT_H

#include <stdint.h>
#include "../runtime/memory.h"   /* PS2_RAM_BASE */

/* Maximum PATH1 count of a full-screen loading picture in this state, or zero
   where a small 3D race view must never be mistaken for a card. The goggles
   transition before a level draws 3,648 primitives; the later tiled card draws
   144..170. The shop exit (18) already shows the goggles before screen 9.
   The flyby and title retain the lower limit, since they also draw 3D. */
static inline int widescreen_flat_limit(void)
{
    int menu = *(volatile int32_t *)(uintptr_t)(PS2_RAM_BASE + 0x29DED8);
    int phase = *(volatile int32_t *)(uintptr_t)(PS2_RAM_BASE + 0x43F348);
    if (menu == 18 || (menu == 9 && phase == 0)) return 4096;
    if (menu == 0 || (menu == 9 && phase == 5)) return 1000;
    return 0;
}

#endif
