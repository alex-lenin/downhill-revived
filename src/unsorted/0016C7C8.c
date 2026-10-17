/* UNIT 0x0016C7C8 0x0016C8A0 */
typedef struct Item {
    char pad0[0x6D0];
} Item;

typedef struct Manager {
    int x0[10];
    int x28[10];
    Item x50[10];
    float x4470;
    float x4474;
    float x4478;
    float x447C;
    float x4480;
    int x4484;
    int x4488;
    int x448C[10];
    int x44B4;
    int x44B8;
    int x44BC;
    int x44C0;
    int x44C4;
    int x44C8;
    char x44CC;
} Manager;

extern void func_0016CDE8(Item *);

void func_0016C7C8(Manager *mgr) {
    int i;

    for (i = 0; i < 10; i++) {
        mgr->x0[i] = 0;
        mgr->x28[i] = 0;
        mgr->x448C[i] = 0;
        func_0016CDE8(&mgr->x50[i]);
    }
    mgr->x4470 = 0.22f;
    mgr->x4474 = 0.29333332f;
    mgr->x4478 = 15.0f;
    mgr->x447C = 15.0f;
    mgr->x4480 = 3.0f;
    mgr->x4484 = 0;
    mgr->x4488 = 0;
    mgr->x44B4 = 0;
    mgr->x44B8 = 0;
    mgr->x44BC = 0;
    mgr->x44C0 = -1;
    mgr->x44C4 = -1;
    mgr->x44C8 = 0;
    mgr->x44CC = 0;
}
