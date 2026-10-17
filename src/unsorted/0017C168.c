/* UNIT 0x0017C168 0x0017C2A0 */
struct Item { char pad[0x4c]; float x4c; };
struct List { char pad[8]; int count; char padc[0x14]; struct Item *items[1]; };
struct Slots { char pad[0x20]; struct List *slots[4]; };
struct Flag { char pad[4]; unsigned char x4; };
struct Obj { char pad[0xf84]; struct Slots *xf84; char padf88[0x1f4]; struct Flag *x117c; char pad1180[0x194]; int x1314; int x1318; char pad131c[0x3a]; unsigned char x1356; };
extern void func_0017C130(struct Obj *);
void func_0017C168(struct Obj *obj) {
    int index = 0;
    int countIndex;
    struct List *list;
    struct Item **item;
    if (obj->x1318 > 0) {
        for (index = 0; index < 4; index++) {
            obj->x1356 = index;
            func_0017C130(obj);
        }
        if (--obj->x1318 == 0) obj->x117c->x4 = 0;
    } else {
        if (obj->x1314 > 0) {
            if (obj->x1356 < 4) func_0017C130(obj);
        } else if (obj->x1356 < 4) {
            obj->x1314 = 10;
            obj->x1356++;
        }
        if (obj->x1356) {
            list = obj->xf84->slots[obj->x1356 - 1];
            countIndex = 1;
            if (countIndex < list->count) {
                item = list->items;
                do {
                    if ((*item)->x4c < 0.4f) (*item)->x4c += 0.05f;
                    countIndex++;
                    item++;
                } while (countIndex < list->count);
            }
        }
    }
}
