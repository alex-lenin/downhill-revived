/* UNIT 0x00154E10 0x00154F40 */
typedef struct Obj {
    char pad0[0x4098];
    unsigned long flags; /* bit 2: skip */
    char pad40A0[0x792C - 0x40A0];
    char *pos;
    char pad7930[0x797C - 0x7930];
    int index;
    char pad7980[0x7BD0 - 0x7980];
} Obj;

extern struct {
    Obj objects[10];
    char pad[0x501A8 - 10 * 0x7BD0];
    int count;
} D_002AF2C0;

extern float func_002487A0(float);

Obj *func_00154E10(Obj **arg, float angle, float radius) {
    float delta[4] __attribute__((aligned(16)));
    Obj *self = *arg;
    Obj *best = 0;
    Obj *p;
    float threshold = func_002487A0(angle);
    int i;

    for (i = 0, p = D_002AF2C0.objects; i < D_002AF2C0.count; i++, p++) {
        if (i != self->index && !((unsigned char)(p->flags >> 2) & 1)) {
            float length, dot, cosine;
            float *forward;
            char *src = p->pos + 0x10;
            char *dst = self->pos + 0x10;

            __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                              : "=m"(delta) : "r"(src), "r"(dst) : "memory");
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                              : "=&f"(length) : "f"(delta[0]), "f"(delta[1]), "f"(delta[2]));
            forward = (float *)(self->pos + 0x50);
            __asm__ volatile ("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6"
                              : "=f"(dot) : "f"(forward[0]), "f"(forward[1]), "f"(forward[2]), "f"(delta[0]), "f"(delta[1]), "f"(delta[2]));
            cosine = dot * (1.0f / length);
            if (threshold < cosine && length < radius) {
                threshold = cosine;
                best = p;
            }
        }
    }
    return best;
}
