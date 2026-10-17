/* UNIT 0x001224F0 0x00122830 */
typedef unsigned int u128 __attribute__((mode(TI)));
class Obj { public: virtual void v0(); char bytes[0x230]; };

/* 0x16C: table reference (table, entry, two -1 indices) */
struct Binding {
    void *x0; void *x4; short x8, xA;
    void Init() { x0 = 0; x4 = 0; xA = 0; x8 = 0; }
    void Set(void *table, void *h);
};
struct Vec { int x0, x4; float x8; int xC; };
struct State { char pad0[0x48]; float x48, x4C; };
struct Tail { char pad0[0x30]; float x30, x34, x38, x3C; };

struct World { char pad0[0x50130]; int x50130; };

extern "C" {
void func_00254204(void *, int, int);
void *func_00163D40(void *, int);
void func_00261078(void *);
void func_00122830(Obj *, void *);
void func_00127170(Obj *, int);
void func_001AC880(void *, float);
void func_001AC7B0(void *, void *);
void func_001AC810(void *, float);
void func_001ACB70(void *, float);
void func_001ACAE0(void *, float);
extern char D_00412510[];
extern float D_0029C1B0[];
extern World D_002AF2C0;
}
inline void Binding::Set(void *table, void *h) {
    x0 = table;
    void *v = h;
    if (!v) v = func_00163D40(table, 0);
    x4 = v;
    xA = -1;
    x8 = -1;
}
/* method table indexed by the init mode */
extern "C" {
extern void (Obj::*D_0026DD10[])();
}

#define I(n) (*(int *)(p + (n)))
#define F(n) (*(float *)(p + (n)))
#define Q(n) (*(u128 *)(p + (n)))
#define Z(n) __asm__ volatile("sqc2 $vf0, %0" : "=m"(Q(n)))
/* sq $zero: gcc materializes a TImode 0 with por first */
#define SQZ(n) __asm__ volatile("sq $0, %0" : "=m"(Q(n)))

extern "C" void func_001224F0(Obj *obj, int owner, int node, int index, int methodIndex) {
    char *p = (char *)obj;
    void *handle;
    Binding *binding;
    State *state;
    Tail *tail;
    Vec *v100, *v110;
    float old;
    float beginning;
    int spawned = 0;
    int *cur;
    int n;
    I(0x130) = owner;
    I(0x134) = 0;
    func_00254204(p + 0x14C, 0, 8);
    func_00254204(p + 0x154, 0, 8);
    I(0x15C) = node;
    I(0x160) = I(0x168) = index >= 0 ? (int)func_00163D40(D_00412510, index) : 0;
    I(0x164) = I(0x168);
    binding = (Binding *)(p + 0x16C);
    binding->Init();
    binding->Set(D_00412510, (void *)I(0x160));
    state = (State *)(p + 0x180);
    func_00261078(state);
    state->x48 = 2.0f;
    state->x4C = 2.0f;
    SQZ(0x1D0);
    tail = (Tail *)(p + 0x1E0);
    SQZ(0x1E0);
    SQZ(0x1F0);
    SQZ(0x1F0);
    v100 = (Vec *)(p + 0x100);
    tail->x30 = 2.0f;
    v110 = (Vec *)(p + 0x110);
    tail->x34 = 14.666667f;
    tail->x3C = 14.666667f;
    tail->x38 = 14.666667f;
    F(0xDC) = 1.0f;
    I(0) = 1;
    I(4) = 0;
    I(8) = 0;
    I(0xC) = 0;
    I(0x18) = 0;
    I(0x3C) = 0;
    I(0x24) = 0;
    I(0x30) = 0;
    n = 9;
    cur = (int *)(p + 0x64);
    do { *cur = 0; --n; --cur; } while (n >= 0);
    beginning = D_0029C1B0[0];
    I(0x28) = 1;
    I(0x2C) = 0;
    I(0x10) = 0;
    I(0xCC) = 0;
    F(0xD8) = beginning;
    F(0xD4) = beginning;
    Z(0xE0);
    Z(0xF0);
    I(0x100) = 0;
    v100->x4 = 0;
    v100->x8 = 1.0f;
    I(0x110) = 0;
    v110->x8 = 1.0f;
    v110->x4 = 0;
    Z(0x120);
    handle = (void *)I(0x160);
    I(0x138) = (int)D_002AF2C0.x50130;
    I(0x13C) = (int)D_002AF2C0.x50130;
    I(0x140) = (int)D_002AF2C0.x50130;
    I(0x144) = (int)D_002AF2C0.x50130;
    I(0x148) = 0;
    if (handle) {
        func_00122830(obj, handle);
        spawned = 1;
    }
    (obj->*D_0026DD10[methodIndex])();
    if (I(0x130)) {
        char *attachment = p + 0x68;
        I(0xD0) = 0;
        old = F(0xD0);
        func_001AC880(attachment, old);
        func_001AC7B0(attachment, (char *)I(0x130) + 0x220);
        *(unsigned char *)(attachment + 9) = 4;
        *(short *)(attachment + 10) = 999;
        func_001AC810(attachment, 200.0f);
        func_001ACB70(attachment, old);
        func_001ACAE0(attachment, 0.5f);
    }
    if (spawned) func_00127170(obj, 0);
}
