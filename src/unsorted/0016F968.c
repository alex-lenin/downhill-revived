/* UNIT 0x0016F968 0x0016F9F8 */
typedef float Vec[4];
typedef struct { char pad0[0x1F0]; char *x1F0; } Obj;
extern void func_001272F8(void *, Vec);
extern void func_001273B0(void *, void *);
void func_0016F968(Obj *obj, void *target, float value) {
 Vec in __attribute__((aligned(16)));
 Vec out __attribute__((aligned(16)));
 char *base = obj->x1F0;
 in[0] = value; in[1] = 0; in[2] = 0;
 __asm__ volatile("lqc2 $vf11,%1\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(out[0]) : "m"(in[0]), "r"(base + 0x40));
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out[0]) : "r"(base + 0x10), "r"(out) : "memory");
 func_001272F8(target, out);
 func_001273B0(target, obj->x1F0 + 0x40);
}
