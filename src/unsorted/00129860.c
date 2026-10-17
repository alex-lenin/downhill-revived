/* UNIT 0x00129860 0x00129C68 .rodata=0x288FA0:0x288FD0 */
typedef struct Ref { int x0,x4,x8,xC; } __attribute__((packed)) Ref;
typedef struct Header { unsigned int flags:7; signed int id:11; unsigned int rest:14; } Header;
typedef struct Item { Header *x0; } Item;
typedef struct Config { char pad0[0x31B8]; float x31B8, x31BC, x31C0; } Config;
typedef struct Handle { char *x0; int x4; short x8, xA; } Handle;
typedef struct Obj {
    int x0; Item *x4, *x8; char padC[0x14]; Config *x20; int x24, x28;
    Ref x2C, x3C, x4C, x5C; Handle x6C; char pad78[0x98]; int x110;
    char pad114[0x40]; float x154, x158; char pad15C[4]; float x160;
    char pad164[0x10]; int x174, x178; char pad17C[4]; int x180, x184;
    char pad188[0x10]; int x198;
} Obj;
extern void func_00203CB8(Ref *);
extern void func_00203DC8(Ref *, int);
extern void func_00203E10(Ref *, float);
extern void func_00202E38(Ref *, int, int, float);
extern void func_0012B540(Obj *, float);
extern int func_00122258(void *, unsigned int);
extern int func_00129058(Config *, int);
extern void func_002182A8(void *, int);
extern void func_0022DDD8(int);
extern int func_00162688(void);
extern char D_003041B0[];

void func_00129860(Obj *self, int state) {
    Ref tmp;
    Handle *h;
    int r;

    if (self->x28 == state) return;
    self->x28 = state;
    self->x174 = 0;
    self->x184 = 0;
    if (self->x3C.x0) { tmp = self->x3C; func_00203CB8(&tmp); }
    if (self->x2C.x0) { tmp = self->x2C; func_00203CB8(&tmp); }
    if (self->x4C.x0) { tmp = self->x4C; func_00203CB8(&tmp); }
    if (self->x5C.x0) { tmp = self->x5C; func_00203CB8(&tmp); }
    switch (self->x28) {
    case 9:
        r = func_00129058(self->x20, func_00122258(D_003041B0, self->x4->x0->rest));
        if (r) {
            h = &self->x6C;
            self->x6C.x0 = D_003041B0;
            h->x4 = r;
            h->xA = -1;
            h->x8 = -1;
            self->x28 = 0;
        } else {
            func_002182A8(self->x4, 0);
            func_002182A8(self->x8, 0);
        }
    case 0:
    case 1:
    case 2:
        func_0022DDD8(self->x110);
        self->x110 = 0;
        break;
    case 3:
        tmp = self->x3C; func_00203DC8(&tmp, 1);
        tmp = self->x3C; func_00203E10(&tmp, self->x154);
        func_00202E38(&self->x3C, 1, 1, 600.0f);
        func_0012B540(self, 0.073333333f);
        return;
    case 4:
        tmp = self->x2C; func_00203DC8(&tmp, 1);
        tmp = self->x2C; func_00203E10(&tmp, self->x160);
        func_00202E38(&self->x2C, 1, 1, 600.0f);
        func_0012B540(self, 0.0f);
        self->x110 = func_00162688();
        break;
    case 5:
        self->x154 = self->x20->x31BC;
        self->x158 = self->x20->x31C0;
        tmp = self->x4C; func_00203DC8(&tmp, 1);
        tmp = self->x4C; func_00203E10(&tmp, self->x154);
        func_00202E38(&self->x4C, 1, 1, 600.0f);
        self->x180 = 0;
        self->x178 = 1;
        func_0012B540(self, self->x20->x31B8);
        return;
    case 6:
        tmp = self->x5C; func_00203DC8(&tmp, 0);
        tmp = self->x5C; func_00203E10(&tmp, 1.0f);
        func_00202E38(&self->x5C, 1, 1, 600.0f);
        func_0012B540(self, 0.0f);
        self->x198 = 0;
        break;
    }
}
