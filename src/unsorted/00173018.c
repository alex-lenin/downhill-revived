/* UNIT 0x00173018 0x00173148 */
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Body {
    char pad0[0x10];
    Vec pos;
} Body;

typedef struct Obj {
    char pad0[0xC];
    Body *xC;
    char pad10[0x4];
    Body *x14;
    char pad18[0x78];
    Vec x90;
    char padA0[0x8];
    float xA8;
    char padAC[0xC];
    float xB8;
    float xBC;
    float xC0;
} Obj;

extern float func_00235EA8(float, float, float, float);
extern void func_00173E58(Obj *, float);
extern void func_00174098(Obj *);
extern void func_00173A18(Obj *);

void func_00173018(Obj *obj) {
    Vec *d;
    float lenSq;

    obj->xB8 = func_00235EA8(obj->xB8, 0.0f, 0.05f, 0.001f);
    d = &obj->x90;
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(obj->x90) : "r"(&obj->x14->pos), "r"(&obj->xC->pos));
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                      : "=f"(lenSq) : "f"(obj->x90.x), "f"(d->y), "f"(d->z));
    if (lenSq > 90000.0f) {
        if (obj->xBC < 0.0f) {
            func_00173E58(obj, -obj->xC0);
        } else {
            func_00173E58(obj, obj->xC0);
        }
        func_00174098(obj);
    }
    func_00173A18(obj);
    if (obj->xC->pos.z - obj->xA8 > 70.0f) {
        obj->xC->pos.z -= 0.5f;
    }
}
