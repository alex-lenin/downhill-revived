/* UNIT 0x00170BC8 0x00170C28 */
#include "player_slot_event.h"
struct Owner { char pad[0x7988]; unsigned int x7988; };
struct Obj { struct Owner *x0; char pad[8]; int xC; };
extern char D_003B2270[];

void func_00170BC8(struct Obj *obj) {
    char *base = D_003B2270;
    int value;
    unsigned int index;
    value = obj->xC + 4;
    obj->xC = value;
    index = obj->x0->x7988;
    if (index < 4) SetPlayerSlotEventLegacy((index * 0x1380 + 0x150) + base, 0x30, 0, value, 0, 0, 0);
}
