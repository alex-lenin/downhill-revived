/* UNIT 0x00142940 0x00142A28 */
struct Params { char pad0[0x1C]; short x1C; short x1E; };
struct Obj {
    float pos[4];
    char pad10[0x40];
    int state;
    char pad54[0x4];
    int x58;
    char pad5C[0x94];
    float *xF0;
};

extern float D_0029DD3C;
extern struct Params D_0026E4F0;
extern void func_002182A8(float *, int);
extern void func_001425B0(struct Obj *, int, int);

void func_00142940(struct Obj *obj) {
    float length;
    float w;
    float *p;

    func_002182A8(obj->xF0, 1);
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
             : "=&f"(length) : "f"(obj->pos[0]), "f"(obj->pos[1]), "f"(obj->pos[2]));
    w = length * 0.9992f + D_0029DD3C * 1.2f - obj->pos[2] * -0.008f / length;
    __asm__ volatile ("lqc2 $vf11,0(%0)\n\tmfc1 $2,%1\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"
                      : : "r"(obj), "f"(w / length) : "$2", "memory");
    obj->pos[3] = w;
    if (obj->x58 == 0) {
        obj->state = 3;
        func_001425B0(obj, D_0026E4F0.x1C, D_0026E4F0.x1E);
    }
    p = obj->xF0;
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x10(%0)"
                      : : "r"(p), "r"(p + 4), "r"(obj) : "memory");
}
