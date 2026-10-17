/* UNIT 0x0017C9F8 0x0017CAA8 */
typedef struct Item {
    char pad0[0x2C];
} Item;

typedef struct Obj {
    Item items[32];
    int count;
    float x584;
} Obj;

extern int D_004B2840[];
extern void func_0017CC58(Item *item);

void func_0017C9F8(Obj *obj)
{
    int i;

    switch (D_004B2840[0]) {
    case 5:
        obj->x584 = 0.1f;
        break;
    case 0x16:
        obj->x584 = 0.01f;
        break;
    default:
        obj->x584 = 0.0f;
        break;
    }
    for (i = 0; i < obj->count; i++) {
        func_0017CC58(&obj->items[i]);
    }
}
