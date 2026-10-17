/* UNIT 0x001BC4F0 0x001BC618 */
typedef struct Slot {
    char pad0[0x18];
    int x18;
    char pad1C[0x14];
    int x30;
    char pad34[0x50];
} Slot;

typedef struct Manager {
    char pad0[0x4];
    char x4[0x610];
    Slot slots[4];
    char pad824[0xC];
    int startTime;
    int x834;
    char x838[0x14];
    char x84C[0x14];
} Manager;

typedef struct Global {
    char pad0[0x5B74];
    char x5B74[1];
} Global;

extern int D_004B26B8[];
extern Global D_00398C40;

static inline Global *global(void) { return &D_00398C40; }

extern void func_001C0E00(void *p);
extern void func_001BBF30(void *p, int index, Slot *slot);
extern int func_00235590(void *p, int index);
extern int func_00238F48(void);
extern void func_001C1420(void *p);

void func_001BC4F0(Manager *mgr)
{
    int i;
    int elapsed;

    if (D_004B26B8[0] != 2) {
        func_001C0E00(mgr->x838);
        func_001C0E00(mgr->x84C);
    }
    for (i = 0; i < 4; i++) {
        if (mgr->slots[i].x18 == 0) {
            func_001BBF30(mgr->x4, i, &mgr->slots[i]);
        }
        if (func_00235590(mgr->x4, i) == 1) {
            mgr->slots[i].x30 = 0;
        }
    }
    elapsed = func_00238F48() - mgr->startTime;
    if (mgr->x834 == 0 && (float)elapsed > 90.0f) {
        func_001C1420(global()->x5B74);
        mgr->x834 = 1;
    }
}
