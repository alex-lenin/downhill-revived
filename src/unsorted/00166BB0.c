/* UNIT 0x00166BB0 0x00166E98 .sdata=0x29C7E8:0x29C7EC */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Obj {
    char pad0[0x120]; int x120;
    char pad124[0x94]; char x1B8[0x64]; char x21C[0x64]; char x280[0x64];
    char x2E4[0x64]; char x348[0x64]; char x3AC[0x64];
};
float D_0029C7E8 = 0.04f;
extern float func_001ACBA0(void *);
extern void func_001ACAE0(void *, float);
extern float func_00236088(float, float, float, float);
static inline void Scale(Vec *out, Vec *v, float scale) {
    __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v), "f"(scale) : "$2");
}
#define LENGTH_SQ(out, v) \
    __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(out) : "f"((v).x), "f"((v).y), "f"((v).z))
/* The zero-byte labels are output-less volatile asms: gcc 2.95's CSE flushes its
   known values at one, so max * max stays an unfolded mul.s like retail. */
void func_00166BB0(struct Obj *obj, Vec *dir, float rate) {
    Vec v;
    float half, length, inv;
    Scale(&v, dir, -D_0029C7E8);
    LENGTH_SQ(length, v);
    half = 0.5f;
    v.w = length;
    __asm__ volatile("Lclamp_166BB0:" : : "f"(length));
    if (half * half < length) {
        __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(half), "f"(length));
        Scale(&v, &v, inv);
    }
    v.x = func_00236088(func_001ACBA0(obj->x1B8), half - v.x, rate, 0.01f);
    func_001ACAE0(obj->x1B8, v.x);
    v.z = func_00236088(func_001ACBA0(obj->x280), v.z + half, rate, 0.01f);
    func_001ACAE0(obj->x280, v.z);
    if (obj->x120 == 0) {
        v.y = func_00236088(func_001ACBA0(obj->x21C), v.y + half, rate, 0.01f);
        func_001ACAE0(obj->x21C, v.y);
    }
}
void func_00166D28(struct Obj *obj, Vec *dir, float rate) {
    Vec v;
    float half, length, inv;
    if (obj->x120 == 0) {
        Scale(&v, dir, -D_0029C7E8);
        LENGTH_SQ(length, v);
        half = 0.5f;
        v.w = length;
        __asm__ volatile("Lclamp_166D28:" : : "f"(rate));
        if (half * half < length) {
            __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(half), "f"(length));
            Scale(&v, &v, inv);
        }
        v.x = func_00236088(func_001ACBA0(obj->x2E4), v.x + half, rate, 0.01f);
        func_001ACAE0(obj->x2E4, v.x);
        v.y = func_00236088(func_001ACBA0(obj->x348), v.y + half, rate, 0.01f);
        func_001ACAE0(obj->x348, v.y);
        v.z = func_00236088(func_001ACBA0(obj->x3AC), v.z + half, rate, 0.01f);
        func_001ACAE0(obj->x3AC, v.z);
    }
}
