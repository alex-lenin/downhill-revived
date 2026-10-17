/* UNIT 0x001447C0 0x00144870 */
typedef struct Item {
    char pad0[0x1C0];
} Item;

typedef struct Mgr {
    int x0;
    char pad4[0xC];
    Item items[12];
    int x1510[12];
    float x1540;
    float x1544;
    float x1548;
    float x154C;
    int x1550;
    int x1554;
    int x1558;
    int x155C;
    int x1560;
} Mgr;

extern void func_00144C18(Item *);

void func_001447C0(Mgr *mgr)
{
    int i;

    mgr->x0 = 0;
    for (i = 0; i < 12; i++) {
        func_00144C18(&mgr->items[i]);
        mgr->x1510[i] = 0;
    }
    mgr->x1544 = mgr->x1540 = 0.36666667f;
    mgr->x1548 = 10.0f;
    mgr->x154C = 12.0f;
    mgr->x1550 = 0;
    mgr->x1554 = 0;
    mgr->x1558 = 0;
    mgr->x155C = 0;
    mgr->x1560 = 1;
}
