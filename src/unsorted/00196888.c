/* UNIT 0x00196888 0x00196960 */
typedef struct Body { char pad0[0x4]; unsigned char x4; char pad5[0xB]; float pos[4]; } Body;
typedef struct Target { char pad0[0x792C]; Body *x792C; } Target;
typedef struct Entry {
    int x0;
    int x4;
    Body *x8;
    Target *xC;
    int x10;
} Entry;
typedef struct Table { char pad0[0xBE8]; int xBE8[1]; } Table;

extern Table D_004348F0;
extern int func_00238F60(void);

void func_00196888(Entry *e)
{
    float v[4] __attribute__((aligned(16)));
    float distSq;

    if (e->x10 <= 0)
        return;
    if (e->xC) {
        __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v), "r"(e->xC->x792C->pos), "r"(e->x8->pos) : "memory");
        __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(distSq) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
        if (distSq > 100.0f)
            e->xC = 0;
        return;
    }
    e->x10 -= func_00238F60();
    if (e->x10 <= 0) {
        e->x8->x4 = 1;
        e->x10 = D_004348F0.xBE8[e->x4];
    }
}
