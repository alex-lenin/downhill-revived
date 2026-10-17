/* UNIT 0x002319C8 0x00231A08 */
typedef struct { float x0, x4, x8; } Vec;
typedef struct { char pad0[0x20]; Vec x20; } Obj;
extern void func_00232950(Obj *);
void func_002319C8(Obj *obj, float x, float y, float z) {
    Vec *p = &obj->x20;
    p->x0 = 1.0f / x;
    p->x4 = 1.0f / y;
    p->x8 = 1.0f / z;
    func_00232950(obj);
}

