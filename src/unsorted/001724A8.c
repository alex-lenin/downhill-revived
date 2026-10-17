/* UNIT 0x001724A8 0x00172580 */
typedef struct Item {
    char pad0[0x130];
} Item;

typedef struct Manager {
    Item x0[7];
    int x850[7];
    float x86C;
    float x870;
    float x874;
    float x878;
    float x87C;
    float x880;
    float x884;
    int x888;
    int x88C;
    int x890;
} Manager;

extern void func_00172820(Item *);

void func_001724A8(Manager *mgr) {
    int i;

    for (i = 0; i < 7; i++) {
        mgr->x850[i] = 0;
        func_00172820(&mgr->x0[i]);
    }
    mgr->x86C = 0.9777778f;
    mgr->x870 = 2.4444444f;
    mgr->x874 = 2.9333334f;
    mgr->x87C = 0.025f;
    mgr->x880 = 0.04f;
    mgr->x884 = 0.029670598f;
    mgr->x888 = 0;
    mgr->x88C = 0;
    mgr->x890 = 1;
}
