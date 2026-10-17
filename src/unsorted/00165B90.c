/* UNIT 0x00165B90 0x00165CD8 .sdata=0x29C810:0x29C814 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Body {
    char pad0[0x18]; float x18;
    char pad1C[0x40-0x1C]; Vec x40, x50;
} Body;
typedef struct Obj {
    char pad0[0x100]; Body *x100;
    char pad104[0x120-0x104]; int x120;
    char pad124[0x130-0x124]; Vec x130;
    char pad140[0x1A4-0x140]; float x1A4; int x1A8;
    char pad1AC[4]; float x1B0;
} Obj;
float D_0029C810 = 0.1f;
extern void func_00223348(Vec *, Vec *);
extern void func_00222F60(Vec *, Vec *, Vec *);
extern void func_00223578(Vec *, Vec *, float);
extern void func_00223210(Vec *, Vec *, Vec *);
extern void func_002230B0(Vec *, Vec *);
extern void func_002232C8(Vec *, Vec *);
extern float func_00235EA8(float, float, float, float);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline void Scale(Vec *out, Vec *v, float factor)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v), "f"(factor) : "$2");
}
void func_00165B90(Obj *obj)
{
    Vec delta, direction, old, target;
    float difference;
    if (obj->x120 == 0) {
        if (obj->x1A8++ < 5) {
            COPY_VECTOR(old, &obj->x100->x50);
            COPY_VECTOR(target, &obj->x130);
            if (old.z < 0.0f) Scale(&target, &target, -1.0f);
            func_00223348(&direction, &obj->x100->x40);
            func_00222F60(&delta, &obj->x100->x50, &target);
            func_00223578(&delta, &delta, D_0029C810);
            func_00223210(&direction, &direction, &delta);
            func_002230B0(&direction, &direction);
            func_002232C8(&obj->x100->x40, &direction);
            difference = obj->x100->x18 - obj->x1B0;
            obj->x1A4 = difference;
            obj->x100->x18 = func_00235EA8(obj->x100->x18, difference, 0.1f, 0.001f);
        }
    }
}
