/* UNIT 0x0019BF38 0x0019BF90 */
#include "timer.h"
extern char D_00759780[];
extern void func_00262490(void);
extern void func_001B0870(void *, int);
void func_0019BF38(char **arg0) {
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00262490), arg0, 1);
 func_001B0870(*arg0 + 0x4d10, 0);
 (*arg0)[0x4d50] = 1;
}
