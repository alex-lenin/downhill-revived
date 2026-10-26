/* Per-update rate audit: finds state that advances once per update instead of once per field
   (the high-frame-rate animation bug, finding 090 FPS1). Off unless DHD_RATE_AUDIT is set; every
   call returns at once when it is off. */
#ifndef DHD_RATE_AUDIT_H
#define DHD_RATE_AUDIT_H

#include <stdint.h>

/* Called at the top of the game's frame tick (func_00238C00, patches_fps.py): compares EE RAM
   with the previous update and accumulates each word's total travel. Reads DHD_RATE_AUDIT (the
   output file), DHD_RATE_AUDIT_SKIP, DHD_RATE_AUDIT_SAMPLES, the optional scene gates
   DHD_RATE_AUDIT_MENU / DHD_RATE_AUDIT_PHASE, DHD_RATE_AUDIT_MIN and DHD_RATE_AUDIT_MAX once.
   Ends the process after the sampled window. */
void rate_audit_update(void);

#endif
