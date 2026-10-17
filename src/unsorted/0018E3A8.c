/* UNIT 0x0018E3A8 0x0018E408 */
#include "menu_items.h"
#include "menu.h"


extern void func_0018D898(MenuItem *, int, int);
void func_0018E3A8(SharedMenu *obj, int arg1, const void *arg2, int value) {
    func_0018D898(&obj->items[obj->x418], arg1, (int)arg2);
    ((SharedMenu *)((obj->x418 * 16) + (unsigned int)obj))->items[0].x6 = value;
    obj->x418++;
}
