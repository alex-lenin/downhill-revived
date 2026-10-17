/* UNIT 0x001401B0 0x00140228 */
#include "animation.h"
typedef AnimationHandle Msg;

extern char D_003F52D0[];

extern int func_001F7A78(void);
extern void func_00190528(char *p, int a, int b);
extern void func_001F79D8(int a, int b);

void func_001401B0(void)
{
    Msg msg;

    if (func_001F7A78() == 0x80) {
        func_00190528(D_003F52D0, 0, 0);
        msg.x8 = 0;
        func_00203590(&msg, 0x2588, 0, 0);
        func_00204080(&msg, 1);
        func_002038E0(&msg, 1);
        func_001F79D8(0, 0x10);
    }
}
