/* UNIT 0x001702C8 0x00170380 */
#include "animation.h"
extern char D_003A6A10[];
extern char D_00285A68[];
extern char D_002839D8[];


void func_001702C8(void *arg0, int arg1)
{
    int count;
    char *base;
    char *s0;
    char *s1;

    if (arg1 == 0xFFFF && arg0 != 0) {
        base = D_003A6A10;
        s1 = base + 0x50;
        s0 = base + 0x6F0;
        count = 9;
        do {
            *(void **)(s0 - 0x690) = D_00285A68;
            *(void **)(s0 - 0x230) = D_002839D8;
            count--;
            func_001AC780((AnimationController *)(s1 + 0x2B8), 2, 1);
            *(void **)s0 = D_002839D8;
            func_001AC780((AnimationController *)(s1 + 0x4E8), 2, 1);
            s0 += 0x6D0;
            s1 += 0x6D0;
        } while (count != -1);
    }
}
