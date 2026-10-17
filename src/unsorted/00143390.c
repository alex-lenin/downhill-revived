/* UNIT 0x00143390 0x00143530 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Emitter {
    Vec4 pos;
    short x10;
} Emitter;

typedef struct Particle {
    char pad0[0x170];
} Particle;

typedef struct System {
    char pad0[0x4];
    int count;
    char pad8[0x30 - 0x8];
    Vec4 x30;
    char pad40[0x70 - 0x40];
    Particle particles[1];
} System;

extern char D_003C2230[];

extern void func_001820C8(void *, Emitter *, float *, float *, float *);
extern void func_002226C8(Vec4 *, float *);
extern int func_00222B08(int, int);
extern void func_00142238(Particle *, Emitter *, Vec4 *);

/* 128-bit vector copy (libvu0-style inline asm) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* sets the x component of VU0 register vfN */
#define SET_VF_X(n, f) \
    __asm__ volatile ("qmfc2.ni $2, $vf" #n "\n\tmfc1 $2, %0\n\tqmtc2.ni $2, $vf" #n : : "f"(f) : "$2")

void func_00143390(System *sys, Emitter *emitter)
{
    float angles[3];
    Vec4 mat[4];
    Vec4 pos;
    float rnd[2];
    Vec4 *row0;
    Vec4 *row2;
    int *bits0;
    int *bits1;
    Vec4 *out;
    int i;

    row0 = &mat[0];
    row2 = &mat[2];
    bits0 = (int *)&rnd[0];
    bits1 = (int *)&rnd[1];
    func_001820C8(D_003C2230, emitter, &angles[0], &angles[1], &angles[2]);
    func_002226C8(row0, angles);
    SET_VF_X(27, -3.0f);
    __asm__ volatile ("lqc2 $vf28, 0x0(%0)\n\tlqc2 $vf29, 0x0(%1)\n\tlqc2 $vf30, 0x0(%2)"
                      : : "r"(&emitter->pos), "r"(row0), "r"(row2));
    SET_VF_X(31, (float)emitter->x10);
    for (i = 0; i < sys->count; i++) {
        /* random floats in [2, 4) */
        *bits0 = (func_00222B08(0x80000001, 0x7FFFFFFF) & 0x7FFFFF) | 0x40000000;
        *bits1 = (func_00222B08(0x80000001, 0x7FFFFFFF) & 0x7FFFFF) | 0x40000000;
        SET_VF_X(11, rnd[0]);
        SET_VF_X(12, rnd[1]);
        out = &pos;
        /* operands of vadda are written fs-last for this assembler */
        __asm__ volatile ("vmula.x ACC, $vf27, $vf31\n\t"
                          "vmadd.x $vf11, $vf11, $vf31\n\t"
                          "vmadd.x $vf12, $vf12, $vf31\n\t"
                          "vadda.xyz ACC, $vf0, $vf28\n\t"
                          "vmaddax.xyz ACC, $vf29, $vf11x\n\t"
                          "vmaddx.xyz $vf11, $vf30, $vf12x\n\t"
                          "sqc2 $vf11, 0x0(%0)"
                          : : "r"(out));
        func_00142238(&sys->particles[i], emitter, &pos);
    }
    COPY_VEC(sys->x30, &emitter->pos);
}
