/* UNIT 0x00191528 0x00191570 */
#include "load_transition.h"
extern char D_003F52D0[];

extern void func_00190900(void *p);

void func_00191528(int enable, int id)
{
    if (id == 0xFFFF && enable) {
        InitializeLoadTransitionDiscard(D_003F52D0);
        func_00190900(D_003F52D0 + 0x64);
        *(int *)(D_003F52D0 + 0x130) = 0;
    }
}
