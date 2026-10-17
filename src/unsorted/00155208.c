/* UNIT 0x00155208 0x00155380 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
struct Rider {
    char pad0[0x4180];
    Vec x4180;
    char pad4190[0x3D4];
    short x4564;
    char pad4566[0x33C6];
    char *x792C;
    char pad7930[0x190];
    float x7AC0;
};
struct Obj {
    struct Rider *x0;
    char pad4[0x7C];
    float x80;
    char pad84[0x30];
    int xB4;
    char padB8[8];
    int xC0;
};
extern int func_00238F60(void);
extern void func_00155E78(struct Obj *, int, struct Rider *);

void func_00155208(struct Obj *p) {
    Vec vec;
    struct Rider *r;
    float scale;
    int threshold, phase;

    p->xC0 += func_00238F60();
    if (p->xC0 > 60) {
        threshold = p->x0->x4564;
        if (p->x80 * 60.0f < threshold) {
            phase = p->xB4 % 4;
            switch (phase) {
            case 0: default: scale = 0.0f; break;
            case 1: scale = 0.75f; break;
            case 2: scale = 0.25f; break;
            case 3: scale = 1.5f; break;
            }
            r = p->x0;
            /* vec.xyz = r->x4180.xyz * scale (VU0) */
            __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0"
                             : "=m"(vec) : "r"(&r->x4180), "f"(scale) : "$2");
            vec.z *= 0.1f;
            /* vec.xyz += (r->x792C + 0x10).xyz (VU0) */
            __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                             : "=m"(vec) : "r"(&vec), "r"(r->x792C + 0x10));
            if (r->x7AC0 == 0.0f) {
                p->xB4++;
                if (p->xB4 > 60) {
                    p->xB4 = 0;
                    func_00155E78(p, 3, r);
                }
            } else {
                p->xB4 = 0;
            }
        } else {
            p->xB4 = 0;
        }
    }
}
