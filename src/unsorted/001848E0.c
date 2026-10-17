/* UNIT 0x001848E0 0x00184938 .rodata=0x28C600:0x28C610 */
#include "menu_items.h"

void func_001848E0(MenuSelectionState *obj, int index) {
    int values[2][2] = { {0, 0}, {1, 0} };
    obj->x10 = values[index][0];
    obj->xC = values[index][1];
}
