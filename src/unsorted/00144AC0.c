/* UNIT 0x00144AC0 0x00144B88 */
typedef struct Item {
    char pad0[0x1AC];
    int selected;
    char pad1B0[0x10];
} Item;

typedef struct Obj {
    int x0;
    char pad4[0xC];
    Item items[12];
    char pad1510[0x44];
    int count;    /* 0x1554 */
    int x1558;
    int current;  /* 0x155C */
} Obj;

extern void func_00144870(Obj *obj);
extern void func_00144B88(Obj *obj, int state);
extern void func_00145368(Item *item);

static inline void SetSelected(Item *item, int selected)
{
    item->selected = selected;
}

void func_00144AC0(Obj *obj)
{
    int i;

    if (obj->x1558 != 0) {
        func_00144870(obj);
        func_00144B88(obj, 0);
        obj->x1558 = 0;
        return;
    }
    SetSelected(&obj->items[obj->current++], 0);
    if (obj->current >= obj->count) {
        obj->current = 0;
    }
    SetSelected(&obj->items[obj->current], 1);
    for (i = 0; i < obj->count; i++) {
        func_00145368(&obj->items[i]);
    }
}
