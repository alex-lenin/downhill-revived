/* UNIT 0x001873C0 0x001873F8 */
#include "menu_items.h"
#include "menu.h"

typedef struct {
    int x0;
} Desc;

extern Desc D_0029DE30;


int func_001873C0(struct SharedMenu *obj, int a, int c)
{
    func_0018E3A8(obj, a, &D_0029DE30, c);
    return obj->x418;
}
