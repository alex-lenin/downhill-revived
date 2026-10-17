/* UNIT 0x00207630 0x00207720 .sdata=0x29D9A0:0x29D9A8 */
/* Static initializer (called as func_00207630(1, 0xFFFF) by func_00207720) for the handler
   table that func_00206E48 dispatches through: rows of 3 handlers. In the original C++ this
   was a global initialized with non-constant entries (D_0029D9A0/D_0029D9A4), which g++
   expands into a walk over the rows with a fresh element pointer per row; the code below
   follows that expansion. */
#include "animation.h"
typedef AnimationHandler Handler;
















Handler D_0029D9A0 = func_002627A0;
Handler D_0029D9A4 = func_002628A0;

extern Handler D_004D0278[6][3];

#define SET_ROW(row, a, b, c)       \
    {                               \
        Handler *p = (row);         \
        *p = (a);                   \
        p++;                        \
        *p = (b);                   \
        p++;                        \
        *p = (c);                   \
        p++;                        \
    }                               \
    (row) += 3

void func_00207630(int initialize, int priority) {
    Handler *row;

    if (priority == 0xFFFF && initialize != 0) {
        row = &D_004D0278[0][0];
        SET_ROW(row, func_00262B48, 0, 0);
        SET_ROW(row, func_00262C20, func_00262D30, func_00262FE0);
        SET_ROW(row, func_00263270, func_00263320, D_0029D9A0);
        SET_ROW(row, func_00206E40, 0, 0);
        SET_ROW(row, func_00263438, func_00263578, D_0029D9A4);
        SET_ROW(row, func_00263860, func_002638D8, func_002639D0);
    }
}
