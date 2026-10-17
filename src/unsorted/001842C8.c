/* UNIT 0x001842C8 0x00184310 */
#include "menu.h"
typedef struct Obj { char pad[0x4F8]; void *x4F8; } Obj;
extern Obj D_003C6420;
extern char D_00285EE8[];

void func_001842C8(int arg0, int arg1) {

    if (arg1 == 0xFFFF && arg0 != 0) {
        func_0018D9D0((SharedMenu *)(&D_003C6420));
        D_003C6420.x4F8 = D_00285EE8;
    }

}

