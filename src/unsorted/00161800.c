/* UNIT 0x00161800 0x00161848 */
#include "menu.h"
struct Global { char pad0[0x4f8]; void *x4f8; };
extern struct Global D_0038D650;
extern char D_002856A0[];

void func_00161800(int arg0, int arg1) {
    if (arg1 == 0xffff && arg0 != 0) {
        func_0018D9D0((SharedMenu *)(&D_0038D650));
        D_0038D650.x4f8 = D_002856A0;
    }
}

