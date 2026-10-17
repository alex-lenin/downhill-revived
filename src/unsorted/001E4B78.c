/* UNIT 0x001E4B78 0x001E4BF0 */
#include "menu.h"
typedef struct Obj { char pad[0x4F8]; void *x4F8; } Obj;
extern Obj D_004C4720, D_004C4C30;
extern char D_004C5130[], D_002882C0[], D_00288348[];

extern void *func_001E47F0(void *);
void func_001E4B78(int arg0, int arg1) {
    if (arg1 == 0xFFFF && arg0 != 0) {
        func_0018D9D0((SharedMenu *)(&D_004C4720));
        D_004C4720.x4F8 = D_002882C0;
        func_0018D9D0((SharedMenu *)(&D_004C4C30));
        D_004C4C30.x4F8 = D_00288348;
        func_001E47F0(D_004C5130);
    }
}
