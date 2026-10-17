/* UNIT 0x0010CB70 0x0010D0C0 */
typedef struct Body {
    char pad0[0x18];
    float x18;
} Body;

typedef struct Rider {
    char pad0[0x11E8];
    int x11E8;
    char pad11EC[0x124C - 0x11EC];
    int x124C;
    char pad1250[0x13D4 - 0x1250];
    unsigned char x13D4;
    char pad13D5[0x40C0 - 0x13D5];
    char x40C0[0x4214 - 0x40C0];
    float x4214;
    char pad4218[0x792C - 0x4218];
    Body *x792C;
    char pad7930[0x7A68 - 0x7930];
    unsigned char x7A68;
    char pad7A69[0x7A78 - 0x7A69];
    float x7A78;
} Rider;

typedef struct Obj {
    Rider *x0;
    char pad4[4];
    Rider *x8;
    char padC[0x80 - 0xC];
    int x80;
    char pad84[0x294 - 0x84];
    float x294;
    char pad298[0x2AC - 0x298];
    float x2AC;
    char pad2B0[0x408 - 0x2B0];
    int x408;
    char pad40C[0x418 - 0x40C];
    int x418;
    char pad41C[0x430 - 0x41C];
    int x430;
    char pad434[0x440 - 0x434];
    long x440;
    long x448;
} Obj;

typedef struct Other {
    char pad0[0x1A0];
    int x1A0[50];
    int x268[50];
    char pad330[0x3F3 - 0x330];
    unsigned char x3F3[50];
} Other;

static inline void set_switch(Other *other, int i, int on) {
    if (other->x3F3[i]) {
        other->x268[i] = on;
    } else {
        other->x1A0[i] = on;
        other->x268[i] = 0;
    }
}

extern int D_0039E7F4[];

static inline int Max(int a, int b)
{
    return (a > b) ? a : b;
}

extern void func_0010D0C0(Obj *);
extern void func_00111368(Obj *, Other *);
extern void func_00111510(Obj *, Other *);
extern void func_0010D7B0(Obj *);
extern void func_0010DEF8(Obj *);
extern void func_0010E1E0(Obj *);
extern int func_00238F60(void);
extern float func_00235EA8(float, float, float, float);
extern float func_00138580(void *);
extern float func_0017C008(void);
extern float func_0012E070(Rider *);
extern int func_00138560(void *);
extern float func_001385A8(void *);

void func_0010CB70(Obj *obj, Other *other)
{
    Rider *rider;
    Rider *rival;
    Rider *self;
    long flags;
    long bits;
    long state;
    float speed;
    float limit;
    float threshold;
    int active;

    obj->x440 &= ~(1L << 38);
    if (obj->x440 & (1L << 40)) {
        func_0010D0C0(obj);
    }
    func_00111368(obj, other);
    func_00111510(obj, other);
    func_0010D7B0(obj);
    if (obj->x80) {
        func_0010DEF8(obj);
    } else {
        func_0010E1E0(obj);
        if (obj->x418) {
            obj->x418 -= func_00238F60();
            if (obj->x418 < 0) {
                obj->x418 = 0;
            }
            bits = obj->x440 | (1L << 35);
            bits |= 1L << 34;
            obj->x440 = bits & ~(1L << 38);
        }
    }
    obj->x0->x4214 = func_00235EA8(obj->x0->x4214, obj->x2AC, 0.07f, 0.001f);
    if (obj->x408 > 180) {
        obj->x448 |= 0x20;
    } else if ((obj->x448 & 0x20) && obj->x408 > 120) {
        obj->x448 |= 0x20;
    } else {
        obj->x448 &= ~0x20;
    }
    if (obj->x448 & 0x20) {
        speed = func_00138580(obj->x0->x40C0);
        speed *= func_0017C008();
        if (speed > 30.0f) {
            if (obj->x440 & (1L << 35)) {
                obj->x440 &= ~(1L << 35);
            }
            if (obj->x440 & (1L << 34)) {
                obj->x440 &= ~(1L << 34);
            }
            if (obj->x408 > 240 && D_0039E7F4[0] != 2) {
                self = obj->x0;
                threshold = 40.0f;
                active = self->x13D4 ? self->x124C : self->x11E8;
                if (active) {
                    threshold = 35.0f;
                }
                if (speed > threshold) {
                    obj->x440 |= 1L << 38;
                }
            }
        }
    }
    flags = obj->x440;
    if ((flags & (1L << 35)) && !((unsigned char)((unsigned long)flags >> 43) & 1)) {
        if (func_0012E070(obj->x0) == 0.0f) {
            set_switch(other, 2, 1);
            set_switch(other, 0, 1);
            return;
        }
    }
    if ((unsigned char)func_00138560(obj->x0->x40C0)) {
        return;
    }
    if (!(obj->x440 & (1L << 38))) {
        rider = obj->x0;
        if (!rider->x7A68) {
            rival = obj->x8;
            if (rival && obj->x294 < 40.0f) {
                if (D_0039E7F4[0] != 0 || obj->x430 > 1800) {
                    if (rider->x792C->x18 - rival->x792C->x18 < 10.0f && rival->x7A78 != 0.0f) {
                        speed = func_00138580(rival->x40C0);
                        if (speed < func_00138580(obj->x0->x40C0)) {
                            obj->x418 = Max(obj->x418, 15);
                            obj->x440 |= 1L << 35;
                            set_switch(other, 2, 1);
                            set_switch(other, 0, 1);
                        }
                    }
                }
            }
        }
    }
    if (obj->x430 > 120 && obj->x8) {
        limit = 0.1f;
        if (func_001385A8(obj->x8->x40C0) < limit && func_001385A8(obj->x0->x40C0) < limit) {
            obj->x440 = (obj->x440 & ~(1L << 38)) | (1L << 34);
        }
    }
    state = obj->x440;
    if (state & (1L << 38)) {
        set_switch(other, 1, 1);
    } else if (state & (1L << 34)) {
        set_switch(other, 0, 1);
    }
}
