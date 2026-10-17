/* UNIT 0x002235B0 0x00223620 */
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Quat;

extern void func_00223210(Quat *out, Quat *a, Quat *b);

/* out = (q * (v.xyz, 0)) * conj(q), func_00223210 = quaternion product */
void func_002235B0(Quat *out, Quat *q, Quat *v)
{
    Quat conj;
    Quat vec;

    /* conj.xyz = -q.xyz (as sceVu0ScaleVector) */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(conj) : "r"(q), "f"(-1.0f) : "$2", "memory");
    conj.w = q->w;
    /* 16-byte vector copy (as sceVu0CopyVector) */
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(vec) : "r"(v) : "$2");
    vec.w = 0.0f;
    func_00223210(out, q, &vec);
    func_00223210(out, out, &conj);
}
