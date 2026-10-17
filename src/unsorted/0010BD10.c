/* UNIT 0x0010BD10 0x0010BE38 */
typedef struct Vec { float v[4]; } Vec __attribute__((aligned(16)));
typedef struct World { char pad[0x792c]; char *x792c; } World;
typedef struct Obj { World *x0; char pad4[0x2c]; Vec x30; char pad40[0x34]; Vec *x74; char pad78[0x1a8]; Vec x220; Vec x230; } Obj;
extern float func_00235EA8(float, float, float, float);
void func_0010BD10(Obj *obj) {
    Vec temp;
    int changed = 0;
    if (obj->x220.v[0] != obj->x230.v[0]) { obj->x220.v[0] = func_00235EA8(obj->x220.v[0], obj->x230.v[0], 0.03f, 0.01f); changed = 1; }
    if (obj->x220.v[1] != obj->x230.v[1]) { obj->x220.v[1] = func_00235EA8(obj->x220.v[1], obj->x230.v[1], 0.03f, 0.01f); changed = 1; }
    if (obj->x220.v[2] != obj->x230.v[2]) { obj->x220.v[2] = func_00235EA8(obj->x220.v[2], obj->x230.v[2], 0.03f, 0.01f); changed = 1; }
    if (changed) {
        Vec *v = &obj->x220;
        char *matrix = obj->x0->x792c + 0x40;
        __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(temp) : "r"(v), "r"(matrix));
        __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, %2\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(obj->x30) : "r"(obj->x74), "m"(temp));
    }
}
