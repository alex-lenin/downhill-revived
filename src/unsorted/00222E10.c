/* UNIT 0x00222E10 0x00222E80 */
#include "rtc.h"

extern void func_002559C8(int);
extern void func_00242FB8(void *, int);
extern char D_00663140[];
extern int D_0029E2C0;
void func_00222E10(int value) {
    RtcClock clock;
    if (!value && func_00248058(&clock)) value = clock.second + clock.minute * 60;
    func_002559C8(value);
    func_00242FB8(D_00663140, value);
    D_0029E2C0 = 0x0379EA54;
}
