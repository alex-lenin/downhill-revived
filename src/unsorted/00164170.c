/* UNIT 0x00164170 0x00164260 */
typedef struct Obj {
    char pad0[0x5C];
    int count;
} Obj;

extern float D_0029C7E4[];
extern float *func_00163D40(Obj *, int);

/* nearest entry of obj to pos (other than exclude) */
float *func_00164170(Obj *obj, float *pos, float *exclude)
{
    float v[4] __attribute__((aligned(16)));
    float length;
    float best;
    float *nearest = 0;
    float *p;
    int i;

    if (pos != 0) {
        best = D_0029C7E4[0];
        for (i = 0; i < obj->count; i++) {
            p = func_00163D40(obj, i);
            if (p != 0 && p != exclude) {
                __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v), "r"(pos), "r"(p) : "memory");
                __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
                if (length < best) {
                    nearest = p;
                    best = length;
                }
            }
        }
    }
    return nearest;
}
