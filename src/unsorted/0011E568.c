/* UNIT 0x0011E568 0x0011E860 .rodata=0x288C70:0x288CA0 */
typedef struct { int x0, x4, x8, xC; } Sound;
typedef struct { void *x0; void *x4; short x8, xA; } Part;
typedef struct { char pad0[0x44]; float x44; } Sub110;
typedef struct Point { float x0, x4, x8; } Point;
typedef struct Obj {
    char pad0[4];
    int x4;
    int x8;
    int xC;
    unsigned int **x10;
    void *x14;
    char pad18[0x18];
    void *x30;
    char pad34[0x18];
    Sound x4C;
    Sound x5C;
    char pad6C[0x10];
    Sound x7C;
    char pad8C[0x70];
    Part xFC;
    char pad108[8];
    Sub110 x110;
    char pad158[0x48];
    int x1A0;
    char pad1A4[0x60];
    float x204;
    char pad208[0x28];
    float x230;
    float x234;
    char pad238[0x24];
    short x25C;
    char pad25E[2];
    short x260;
    short x262;
} Obj;
extern void func_0011E860(Obj *);
extern void func_00202E38(void *, int, int, float);
extern void func_00203DC8(Sound *, int);
extern void func_00203E10(Sound *, float);
extern void func_001220B0(Obj *, float);
extern int func_00162688(void);
extern void func_0022DDD8(int);
extern void func_002182A8(void *, int);
extern Point *func_0011CD58(void *, int);
extern int func_00122258(void *, int);
extern char D_003041B0[];
void func_0011E568(Obj *o, int state) {
    Sound sound;
    Point *p;
    Part *part;
    Sub110 *s;
    short delay;
    if (o->x8 == state) return;
    o->x8 = state;
    o->x25C = 0;
    o->x260 = 0;
    switch (state) {
    case 0:
    case 1:
        func_0011E860(o);
        if (o->x4 == 9)
            func_00202E38(&o->x5C, 1, 1, 600.0f);
        goto done;
    case 2:
    done:
        func_0022DDD8(o->x1A0);
        o->x1A0 = 0;
        break;
    case 3:
        func_0011E860(o);
        sound = o->x4C;
        func_00203DC8(&sound, 1);
        sound = o->x4C;
        func_00203E10(&sound, o->x230);
        func_00202E38(&o->x4C, 1, 1, 600.0f);
        func_001220B0(o, o->x204);
        break;
    case 4:
        func_0011E860(o);
        sound = o->x5C;
        func_00203DC8(&sound, 1);
        sound = o->x5C;
        func_00203E10(&sound, o->x234);
        func_00202E38(&o->x5C, 1, 1, 600.0f);
        o->x1A0 = func_00162688();
        break;
    case 5:
        delay = o->x262;
        o->x262 = delay > 0 ? delay : 1;
        break;
    case 6:
        func_0011E860(o);
        o->xC = 1;
        sound = o->x7C;
        func_00203DC8(&sound, 0);
        sound = o->x7C;
        func_00203E10(&sound, 1.0f);
        func_00202E38(&o->x7C, 1, 1, 600.0f);
        s = &o->x110;
        s->x44 = 0;
        func_001220B0(o, s->x44);
        break;
    case 7:
    case 8:
        break;
    case 9:
        func_0011E860(o);
        p = func_0011CD58(o->x30, func_00122258(D_003041B0, **o->x10 >> 18));
        if (p) {
            part = &o->xFC;
            o->xFC.x0 = D_003041B0;
            part->x4 = p;
            part->xA = -1;
            part->x8 = -1;
            o->x8 = 0;
        } else {
            func_002182A8(o->x10, 0);
            if (o->x14)
                func_002182A8(o->x14, 0);
        }
        func_0022DDD8(o->x1A0);
        o->x1A0 = 0;
        break;
    }
}
