/* UNIT 0x0020E990 0x0020EB30 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct {
    float xyz[4];
    union {
        u128 q;
        struct { float x, y, z; int flags; } f;
    } x10;
} Fog;

extern Fog D_004F3200;
extern u128 D_004F32E0[][2];
extern float D_004F33A0[], D_004F33B8[], D_004F33D0[];
extern float D_00279A00[], D_00279A10[];

/* VU0 macro-mode helpers (inline asm, as the libvu0-style original) */
#define SCALE(dst, src, value, mask) \
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx." mask " $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)" \
                      : : "r"(dst), "r"(src), "f"(value) : "$2")
#define BLEND(dst, a, b, value, mask) \
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax." mask " ACC,$vf11,$vf0x\n\tvmaddx." mask " $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" \
                      : "=m"(*(u128 *)(dst)) : "r"(a), "r"(b), "f"(value) : "$2")
#define MIN(dst, a, b, mask) \
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvmini." mask " $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" \
                      : "=m"(*(u128 *)(dst)) : "r"(a), "r"(b))
#define MUL(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%3)\n\tvmul.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" \
                      : "=m"(*(u128 *)(dst)) : "r"(dst), "r"(a), "r"(b))

void func_0020E990(int index, Fog *src, Fog *dst) {
    u128 temp, blend, product;
    float ratio, weight, weight2;

    if (D_004F3200.x10.f.flags || src->x10.f.flags) {
        SCALE(dst, &D_004F3200, D_004F33A0[index], "xyz");
        dst->x10.q = D_004F3200.x10.q;
        D_004F3200.x10.f.flags = 0;
        if (src->x10.f.flags) {
            weight = D_004F33B8[index];
            ratio = weight / D_004F33D0[index];
            SCALE(&temp, src, ratio, "xyzw");
            weight = 1.0f - weight;
            BLEND(&blend, &temp, dst, weight, "xyzw");
            MIN(dst, dst, &blend, "xyzw");

            weight2 = D_004F33B8[index];
            ratio = weight2 / D_004F33D0[index];
            SCALE(&temp, &src->x10, ratio, "xyz");
            MUL(&product, &dst->x10, D_00279A00);
            MUL(&temp, &product, &temp);
            BLEND(&dst->x10, &temp, &dst->x10, 1.0f - weight2, "xyz");
            MIN(&dst->x10, &dst->x10, D_00279A10, "xyz");

            /* sq $zero: gcc always materializes a TImode 0 with por first */
            __asm__ volatile ("sq $0, %0" : "=m"(D_004F32E0[index][0]));
            __asm__ volatile ("sq $0, %0" : "=m"(D_004F32E0[index][1]));
            D_004F33B8[index] = 0;
            D_004F33D0[index] = 0;
        }
    }
}
