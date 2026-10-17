/* UNIT 0x001731C8 0x001732C0 */
typedef struct Frame {
    float v[8];
} __attribute__((aligned(16))) Frame;

typedef struct Obj {
    char pad0[4];
    int x4;
    char pad8[4];
    char *xC;
    char pad10[0x10];
    Frame x20;
    int x40;
    void *x44;
    Frame *x48;
    void *x4C;
    char pad50[0x30];
    float x80[4];
    char pad90[0x24];
    float xB4;
} Obj;

extern char D_003B01E0[];
extern Frame *func_00163E98(void *mesh, void *node);
extern Frame *func_001640D0(void *mesh, void *node, void *ref);

void func_001731C8(Obj *obj)
{
    float *v = obj->x80;
    float x, y, z, length, square, scale;
    Frame *next;

    next = obj->x48;
    obj->x40 = (int)obj->x44;
    obj->x44 = next;
    obj->x20 = *next;
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x80(%0)" : : "r"(obj), "r"(&obj->x20), "r"(obj->xC + 0x10) : "memory");
    x = v[0];
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(x), "f"(v[1]), "f"(v[2]));
    obj->xB4 = length;
    y = v[1]; z = v[2];
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(square) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(scale) : "f"(1.0f), "f"(square));
    v[0] = x * scale; v[1] = y * scale; v[2] = z * scale;
    if (obj->x4 == 2) {
        obj->x48 = func_001640D0(D_003B01E0, obj->x44, obj->x4C);
    } else {
        obj->x48 = func_00163E98(D_003B01E0, obj->x44);
    }
}
