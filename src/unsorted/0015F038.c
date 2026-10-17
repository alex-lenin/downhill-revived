/* UNIT 0x0015F038 0x0015F0C8 */
struct Values {
    int x0;
    int x4;
    int x8;
    char padC[0x1B];
    unsigned char x27;
};

struct Obj {
    char pad0[0x510];
    int x510;
    int x514;
    int x518;
    int x51C;
    int x520;
    int x524;
};

struct Global {
    char pad0[0x11A60];
    struct Values x11A60;
};

struct Index {
    int x0;
    char pad4[8];
};

extern char D_004B2914[];
extern struct Global D_004225A0;
extern struct Index D_003F5400;
extern void func_001C6558(void *);
extern int func_002205A0(int);
extern void func_00220198(int, int);
extern void func_0018E468(struct Obj *);

static inline struct Values *get_values(struct Global *g) {
    return &g->x11A60;
}

void func_0015F038(struct Obj *p) {
    struct Values *v;

    func_001C6558(D_004B2914);
    v = get_values(&D_004225A0);
    v->x8 = p->x510;
    v->x0 = p->x518;
    v->x4 = p->x514;
    v->x27 = p->x51C;
    func_00220198(func_002205A0(D_003F5400.x0), p->x520 != 0);
    p->x524 = 1;
    func_0018E468(p);
}
