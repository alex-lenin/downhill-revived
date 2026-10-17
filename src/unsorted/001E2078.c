/* UNIT 0x001E2078 0x001E20C0 */
#include "menu.h"
struct Obj { char pad[0x4F8]; void *x4F8; };
extern struct Obj D_004C3230;
extern char D_00288018[];

void func_001E2078(int arg0, int arg1) {
    if (arg1 == 0xFFFF && arg0 != 0) {
        func_0018D9D0((SharedMenu *)(&D_004C3230));
        D_004C3230.x4F8 = D_00288018;
    }
}

