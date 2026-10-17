/* UNIT 0x001F9620 0x001F96C8 .sbss=0x29E148:0x29E150 */
#include "menu.h"
#include "animation.h"
struct Obj { char pad[0x4f8]; void *x4f8; char pad4fc[0x14]; int x510; char pad514[0x58]; int x56c; };
extern struct Obj D_004C7530, D_004C7A30, D_004C7F50;
extern char D_002884B8[], D_00288650[], D_002885C8[], D_00275CF0[];
void *D_0029E148 __attribute__((section(".sbss"))) = 0;


void func_001F9620(int a, int b) {
    if (b == 0xffff && a != 0) {
        func_0018D9D0((SharedMenu *)(&D_004C7530));
        D_004C7530.x4f8 = D_002884B8;
        func_0018D9D0((SharedMenu *)(&D_004C7A30));
        D_004C7A30.x4f8 = D_00288650;
        D_004C7A30.x510 = 0;
        func_0018D9D0((SharedMenu *)(&D_004C7F50));
        D_004C7F50.x4f8 = D_002885C8;
        func_001AC780((AnimationController *)((char *)&D_004C7F50 + 0x500), 2, 1);
        D_004C7F50.x56c = 0;
        D_0029E148 = D_00275CF0;
    }
}
