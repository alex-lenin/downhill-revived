/* UNIT 0x00143E38 0x00143F10 */
typedef struct Item {
    char pad0[0x110];
} Item;

typedef struct Obj {
    Item items[7];
    int x770[7];
    float x78C;
    float x790;
    float x794;
    float x798;
    float x79C;
    int x7A0;
    int x7A4;
    int x7A8;
} Obj;

typedef struct Global {
    char pad0[0x2A0];
    int x2A0;
} Global;

extern Global D_004B25A0;

extern void func_00144238(Item *item);

void func_00143E38(Obj *obj)
{
    int i;

    for (i = 0; i < 7; i++) {
        func_00144238(&obj->items[i]);
        obj->x770[i] = 0;
    }
    obj->x78C = (float)0.19555555;
    obj->x790 = (float)0.24444444;
    obj->x79C = 2.2f;
    if (D_004B25A0.x2A0 == 3) {
        obj->x794 = 5.0f;
        obj->x798 = 30.0f;
    } else {
        obj->x794 = 1.0f;
        obj->x798 = 30.0f;
    }
    obj->x7A0 = 0;
    obj->x7A4 = 0;
    obj->x7A8 = 1;
}
