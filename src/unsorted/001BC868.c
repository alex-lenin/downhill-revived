/* UNIT 0x001BC868 0x001BC968 .sdata=0x29D54C:0x29D56C */
typedef struct Entry {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    char pad28[0x2C - 0x28];
    int x2C;
    char pad30[0x34 - 0x30];
    char x34[0x5C - 0x34];
    char x5C[0x84 - 0x5C];
} Entry;

typedef struct Sys {
    int x0;
    char x4[0x610];
    Entry entries[4];
} Sys;

typedef struct Global {
    char pad0[0x5350];
    Sys sys;
} Global;

float D_0029D54C = 0.5f;
float D_0029D550 = 2.2f;
float D_0029D554 = 94.0f;
float D_0029D558 = 575.0f;
float D_0029D55C = 0.3f;
float D_0029D560 = 2.0f;
float D_0029D564 = 40.0f;
float D_0029D568 = 260.0f;

extern Global D_00398C40;
extern int D_004B2828[];

extern void func_002356E0(void *, int, int);
extern void func_001DB920(int, void *, void *);
extern void func_001C2280(void *, float, float, float, float);

void func_001BC868(Sys *sys, int slot, int xC, int x10, int x14, int x4, int x0, int x8,
                   int x18, int x1C, int arg10)
{
    Global *g = &D_00398C40;
    int buf[4];

    sys->entries[slot].x0 = x0;
    sys->entries[slot].xC = xC;
    sys->entries[slot].x10 = x10;
    sys->entries[slot].x8 = x8;
    sys->entries[slot].x14 = x14;
    sys->entries[slot].x18 = x18;
    sys->entries[slot].x4 = x4;
    sys->entries[slot].x1C = x1C;
    sys->entries[slot].x20 = 1;
    sys->entries[slot].x24 = 0;
    sys->entries[slot].x2C = 0;
    func_002356E0(g->sys.x4, slot, arg10);
    if (D_004B2828[0] == 1) {
        func_001DB920(sys->entries[slot].x8, buf, &sys->entries[slot].x2C);
    }
    func_001C2280(sys->entries[slot].x5C, D_0029D55C, D_0029D560, D_0029D564, D_0029D568);
    func_001C2280(sys->entries[slot].x34, D_0029D54C, D_0029D550, D_0029D554, D_0029D558);
}
