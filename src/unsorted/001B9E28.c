/* UNIT 0x001B9E28 0x001B9E88 */
#include "load_queue.h"
extern char D_004D0368[], D_0029D490[], D_0029D498[], D_0029D480[];
extern void *D_004D1A04[];
extern void func_00230EC0(void *, int);
extern void func_00230F80(void *, void *, void *, void *, int, int);

void func_001B9E28(void) {
 char obj[0x150];
 D_004D1A04[0]=D_004D0368;
 func_00230EC0(obj,0);
 func_00230F80(obj,D_0029D490,D_0029D498,D_0029D480,0,0);
 ProcessLoadQueueLegacy(obj);
}
