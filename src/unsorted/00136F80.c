/* UNIT 0x00136F80 0x00137060 */
typedef struct World {
    char pad0[0x4180];
    float x4180[4];
    char pad4190[0x7B00 - 0x4190];
    int x7B00;
} World;

typedef struct Obj {
    char pad0[0x150];
    float x150;
    char pad154[0x158 - 0x154];
    World *x158;
} Obj;

extern float D_004354F8[];
extern float func_00238F38(void);
extern void func_00231C20(Obj *obj, float *v);

void func_00136F80(Obj *obj)
{
    float tmp[4] __attribute__((aligned(16)));
    float t;
    float scale;
    float *v;
    float x, y, z, len, inv;
    int n;

    if (obj->x158->x7B00 == 0) {
        return;
    }
    t = func_00238F38() * 0.5f;
    scale = obj->x150 * t * D_004354F8[0];
    v = obj->x158->x4180;
    x = v[0];
    y = v[1];
    z = v[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    tmp[0] = x * inv;
    tmp[1] = y * inv;
    tmp[2] = z * inv;
    __asm__ volatile ("lqc2 $vf11,0(%0)\n\tmfc1 $2,%1\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"
                      : : "r"(tmp), "f"(scale) : "$2", "memory");
    func_00231C20(obj, tmp);
    n = obj->x158->x7B00 - 1;
    if (n < 0) {
        n = 0;
    }
    obj->x158->x7B00 = n;
}
