/* UNIT 0x001AF5E8 0x001AF770 */
/* Sub is the same object as in func_00131548/func_00136AA8: the 0xFFFF zero-extend on
   the type bitfield needs the struct to be 8-aligned (the long at 0x4B0). */
struct Sub {
    char pad0[0x394];
    unsigned short x394;
    unsigned short type : 5;
    unsigned short x396_5 : 11;
    char pad398[0x3C];
    unsigned char x3D4;
    char pad3D5[0xDB];
    long x4B0;
};
struct Board { char pad[0x40C0]; Sub x40C0; };
struct Value { char pad[0x44]; float x44; char pad48[0x1C]; };
struct Obj { int x0, x4; Board *x8; Value xC, x70; };
struct Entry { float x0, x4; char pad8[0x18]; };

extern Entry D_00768098[];
extern float func_001385A8(Sub *);
extern float func_00236088(float cur, float target, float rate, float min);
extern void func_001ACB70(Value *, float);

static inline float Min(float a, float b) { return a <? b; }
static inline float Max(float a, float b) { return a >? b; }
static inline Value *First(Obj *o) { return &o->xC; }
static inline float Get(Value *p) { return p->x44; }
static inline Entry *GetEntry(int type) {
    if (type >= 32) return D_00768098;
    return &D_00768098[type];
}

void func_001AF5E8(Obj *obj) {
    float scale, amount, old;
    Entry *entry;
    Sub *sub;
    Value *v;

    if (obj->x8) {
        scale = Min(func_001385A8(&obj->x8->x40C0), 0.92f);
        /* read through its own temporary, then v = First(obj) again: gives retail's
           a0 pointer for the load copied into s2 for v */
        amount = Get(First(obj));
        sub = &obj->x8->x40C0;
        entry = GetEntry(obj->x8->x40C0.type);
        v = First(obj);
        if (sub->x3D4 == 2 || entry->x4 < 1.0f)
            amount = func_00236088(amount, 0.0f, 0.4f, 0.2f);
        else
            amount = func_00236088(amount, 1.0f, 0.4f, 0.2f);
        old = amount;
        amount *= scale;
        amount = amount * 0.01f * entry->x4;
        amount = Min(Max(amount, 0.0f), old);
        amount = amount * 0.05f + old * 0.95f;
        v->x44 = amount;
        func_001ACB70(v, scale);
        v = &obj->x70;
        v->x44 = amount;
        func_001ACB70(v, scale);
    }
}
