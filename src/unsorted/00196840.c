/* UNIT 0x00196840 0x00196880 */
typedef struct Item { char pad[12]; float xC; } Item;
typedef struct Obj { int x0, x4; Item *x8; int xC, x10; } Obj;
extern Item *func_00208FB8(void);
void func_00196840(Obj *p) {
    Item *item;
    p->x4 = 0;
    item = func_00208FB8();
    p->x8 = item;
    p->x10 = 0;
    p->xC = 0;
    item->xC = 500.0f;
}

