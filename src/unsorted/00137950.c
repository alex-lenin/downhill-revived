/* UNIT 0x00137950 0x00137B70 */
typedef struct Entry { char pad[0x18]; float x18; char pad1C[4]; } Entry;
typedef struct Sub3530 { char pad0[0xA30]; float xA30; } Sub3530;
typedef struct Sub78D8 { char pad0[0x40]; float x40; } Sub78D8;
typedef struct Sub78D4 { char pad0[0x2B0]; float x2B0; } Sub78D4;
typedef struct State {
    char pad0[0x78D4];
    Sub78D4 *x78D4;
    char pad78D8[0x7A5C - 0x78D8];
    int x7A5C;
} State;
typedef struct Obj {
    char pad0[0x158];
    State *x158;
    char pad15C[4];
    char x160[0x50];
    char pad1B0[0x206 - 0x1B0];
    unsigned short type : 5;
    unsigned short x206_5 : 11;
    char pad208[0x248 - 0x208];
    float x248;
    char pad24C[0x438 - 0x24C];
    float x438;
    float x43C;
    char pad440[0x458 - 0x440];
    float x458;
    float x45C;
    char pad460[0x468 - 0x460];
    float x468;
    float x46C;
    char pad470[0x484 - 0x470];
    float x484;
    char pad488[0x49C - 0x488];
    float x49C;
    char pad4A0[0x4B0 - 0x4A0];
    long x4B0;
} Obj;

extern Entry D_00768098[];
extern int func_00138560(void *p);
extern float func_0016AE18(void *p);
extern float func_0012DEB0(void *p);
extern int func_001E64C8(void *p);
extern int func_001E7BD8(void *p);
extern float fabsf(float);

static inline Entry *GetEntry(int type) {
    if (type >= 32) return D_00768098;
    return &D_00768098[type];
}

void func_00137950(Obj *obj) {
    float accel;
    float input;
    float decay;
    float k;
    float vel;

    if (func_00138560(obj)) {
        float f0 = obj->x49C;
        decay = obj->x46C;
        accel = obj->x45C * f0;
        obj->x484 = obj->x458 * f0;
    } else {
        int type = obj->type;
        Entry *entry = GetEntry(type);
        void *s0 = (char *)obj + 0x160;
        float r1 = func_0016AE18(s0);
        obj->x484 = obj->x438 * r1;
        {
            float r2 = func_0016AE18(s0);
            float e = entry->x18 * 0.01f;
            decay = obj->x468;
            accel = obj->x43C * r2 * e;
        }
    }
    {
        State *st = obj->x158;
        if (st != 0 && st->x7A5C == 3) {
            Sub78D4 *p = st->x78D4;
            if (p != 0) accel *= p->x2B0;
        }
        {
            char *sub = (char *)obj->x158 + 0x3530;
            obj->x484 *= ((Sub3530 *)sub)->xA30;
            accel *= ((Sub3530 *)sub)->xA30;
        }
    }
    if (func_00138560(obj) == 0) {
        char *p2 = (char *)obj->x158 + 0x78D8;
        obj->x484 *= ((Sub78D8 *)p2)->x40;
        accel *= ((Sub78D8 *)p2)->x40;
    }
    input = func_0012DEB0(obj->x158);
    if (func_001E64C8((char *)obj->x158 + 0x6610) && !func_001E7BD8((char *)obj->x158 + 0x5FF0)) input = 0.0f;
    /* Zero-byte asm labels (cf. src/unsorted/00111D98.cpp): scheduling
       barriers that keep retail's source order around abs.s (store, abs,
       then the cap load / eps constant). Their operands shape allocation:
       step stays live past the add so the sum takes vel's $f1, the sums stay
       live past abs.s so it gets its own register, and k is one variable for
       the 0.0f ($f3) and the speed cap so both share $f3. "+f" (k) keeps the
       final store as swc1 $f3 instead of sw $zero. */
    k = 0.0f;
    vel = obj->x248;
    if (input != k) {
        float a;
        float step = accel * input;
        float next;
        next = vel + step;
        __asm__ volatile ("L37950_a0:" : : "f" (step));
        obj->x248 = next;
        __asm__ volatile ("L37950_a1:");
        a = fabsf(next);
        __asm__ volatile ("L37950_a2:" : : "f" (next));
        k = obj->x484;
        if (k < a) obj->x248 = k * input;
    } else {
        obj->x49C = 1.0f;
        if (vel != k) {
            float a;
            float next = vel * decay;
            obj->x248 = next;
            __asm__ volatile ("L37950_b1:");
            a = fabsf(next);
            __asm__ volatile ("L37950_b2:" : "+f" (k) : "f" (next));
            if (a < 0.00014544411f) obj->x248 = k;
        }
    }
}
