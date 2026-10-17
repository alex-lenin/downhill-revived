/* UNIT 0x0010C4C0 0x0010C5D8 */
typedef struct Sub74 {
    char pad0[0x14];
    unsigned short x14;
} Sub74;

typedef struct Obj {
    unsigned char *x0;
    char pad4[0x70];
    Sub74 *x74;
    char pad78[0x430 - 0x78];
    int x430;
    char pad434[0x440 - 0x434];
    long x440;
    long x448;
} Obj;

typedef struct Other {
    char pad0[0x1A4];
    int x1A4[50];
    int x26C[50];
    char pad334[0x3F4 - 0x334];
    unsigned char x3F4[50];
} Other;

static inline void set_switch(Other *other, int i, int on) {
    if (other->x3F4[i]) {
        other->x26C[i] = on;
    } else {
        other->x1A4[i] = on;
        other->x26C[i] = 0;
    }
}

extern int func_00238F60(void);
extern void func_00111B18(Obj *);
extern void func_001128A8(Obj *);
extern void func_001132B0(Obj *);
extern int func_00138560(unsigned char *);
extern void func_00112360(Obj *);
extern void func_0010EC80(Obj *);
extern void func_00113958(Obj *);
extern void func_0010C5D8(Obj *, Other *);
extern void func_0010C940(Obj *, Other *);
extern void func_0010CAF0(Obj *, Other *);

void func_0010C4C0(Obj *obj, Other *other) {
    obj->x430 += func_00238F60();
    func_00111B18(obj);
    func_001128A8(obj);
    func_001132B0(obj);
    if (!(unsigned char)func_00138560(obj->x0 + 0x40C0)) {
        func_00112360(obj);
    }
    func_0010EC80(obj);
    func_00113958(obj);
    if (!(obj->x440 & 0x100000000)) {
        set_switch(other, 0, 1);
    } else if (((obj->x74->x14 >> 5) & 1) && obj->x0[0x7A68]) {
        func_0010C5D8(obj, other);
    } else if (obj->x448 & 0x10) {
        func_0010C940(obj, other);
    } else {
        func_0010CAF0(obj, other);
    }
}
