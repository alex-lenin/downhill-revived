/* UNIT 0x00174968 0x00174C58 */
typedef struct Big Big;
typedef struct Obj {
    Big *x0;
    char pad4[8];
    int xC;
    float x10;
    float x14;
    float x18;
    char pad1C[8];
    int x24;
    unsigned char x28;
    unsigned char x29;
    unsigned char x2A;
    unsigned char x2B;
    unsigned char x2C;
} Obj;

struct Vtable {
    char pad[0x20];
    short delta;
    short pad2;
    int (*call)(void *);
};

struct Virtual {
    int x0;
    struct Vtable *vt;
};

typedef struct Inner {
    char pad0[0x498];
    float x498;
    float x49C;
    char pad4A0[4];
    short x4A4;
    char pad4A6[9];
    unsigned char x4AF;
} Inner;

struct Big {
    char pad0[0x11A8];
    int x11A8;
    char pad11AC[0xC4];
    int x1270;
    char pad1274[0x169];
    unsigned char x13DD;
    char pad13DE[0x2CE2];
    Inner x40C0;
    char pad4570[0x960];
    char x4ED0[0x1120];
    struct Virtual x5FF0;
    char pad5FF8[0x2E8];
    struct Virtual x62E0;
    char pad62E8[0x28];
    struct Virtual x6310;
    char pad6318[0x18];
    char x6330[0x50];
    int x6380;
    char pad6384[0x1820];
    int x7BA4;
    char pad7BA8[8];
    unsigned char x7BB0;
};

extern int func_00238F60(void);
extern float func_00236088(float, float, float, float);
extern void func_0012FE38(void *, void *);
extern void func_00136030(Inner *, int);
extern void func_00174C58(Obj *);
extern void func_00174D48(Obj *);
extern void func_001750A8(Obj *);
extern void func_00170BC8(void *);
extern void func_001AC578(void *);
extern float D_0029C374;
extern float D_0029C378;
extern int D_0029D740;

static inline void SetA(Inner *p, float v) { p->x498 = v; }
static inline void SetB(Inner *p, float v) { p->x49C = v; }
static inline float GetX(Inner *p) { return p->x4A4; }

void func_00174968(Obj *obj) {
    int state = obj->xC;
    Big *b = obj->x0;
    int v;
    int lim;
    int cap;
    int cond;
    float mult;
    struct Virtual *p1;
    struct Virtual *p2;
    struct Virtual *p3;

    switch (state) {
    case 1:
        v = obj->x24 + func_00238F60();
        lim = 299;
        cap = 300;
        v = lim < v ? cap : v;
        obj->x24 = v;
        obj->x10 = func_00236088(obj->x10, -0.8f, 0.09f, 0.050000004f);
        obj->x14 = func_00236088(obj->x14, 1.0f, 0.09f, 0.045f);
        obj->x18 = func_00236088(obj->x18, 0.3f, 0.09f, 0.050000004f);
        if (b->x13DD != 0) {
            cond = b->x1270;
        } else {
            cond = b->x11A8;
        }
        if (cond == 0) {
            obj->xC = 2;
            if (b->x7BA4 != 0) {
                if (obj->x2C != 0) {
                    b->x6380 = 12;
                }
                func_00136030(&b->x40C0, 1);
                b->x7BA4 = 0;
            }
        }
        func_00174C58(obj);
        func_00174D48(obj);
        break;
    case 2:
        obj->xC = 0;
        if (GetX(&b->x40C0) < 15.0f) {
            mult = 1.0f;
            if (b->x7BB0 != 0) {
                mult = 2.0f;
            }
            if (obj->x28 != 0 && obj->x29 != 0) {
                SetA(&b->x40C0, D_0029C374 * mult);
            } else if (obj->x2A != 0 && obj->x2B != 0) {
                SetB(&b->x40C0, D_0029C378 * mult);
            } else if (D_0029D740 != 0) {
                if (obj->x28 != 0) {
                    SetA(&b->x40C0, D_0029C374 * mult);
                } else if (obj->x29 != 0) {
                    SetA(&b->x40C0, D_0029C374 * mult);
                } else if (obj->x2A != 0) {
                    SetB(&b->x40C0, D_0029C378 * mult);
                } else if (obj->x2B != 0) {
                    SetB(&b->x40C0, D_0029C378 * mult);
                }
            }
        }
        func_0012FE38(b, b->x4ED0);
        break;
    case 0:
        func_0012FE38(b, b->x4ED0);
        break;
    }
    func_001750A8(obj);
    func_001AC578((char *)b + 0x1E40);
    p1 = &b->x5FF0;
    if (p1->vt->call((char *)p1 + p1->vt->delta) != 0) {
        func_0012FE38(b, p1);
    } else {
        p2 = &b->x62E0;
        if (p2->vt->call((char *)p2 + p2->vt->delta) != 0) {
            func_0012FE38(b, p2);
        } else {
            p3 = &b->x6310;
            if (p3->vt->call((char *)p3 + p3->vt->delta) != 0) {
                func_0012FE38(b, p3);
            }
        }
    }
    if (b->x7BA4 != 0) {
        func_00170BC8(b->x6330);
    }
}
