/* UNIT 0x00235E18 0x00235E70 */
#include "scalar_api.h"
typedef struct {
    int x0;
    char pad4[0x3908];
    int x390C;
    int x3910;
    int pad3914;
    int x3918;
    int pad391C;
    int x3920;
} State;

void func_00235E18(int arg, ScalarUnsigned64 base)
{
    State *s = (State *)(int)base;

    s->x3918 = arg;
    if (arg == 0) {
        s->x3910 = s->x390C;
        s->x3920 = -1;
        if (s->x0 != 5) {
            s->x0 = 1;
        }
    } else {
        if (s->x0 != 5) {
            s->x0 = 3;
        }
    }
}
