/* UNIT 0x002093C8 0x00209460 */
extern void func_002092B8(void *,void *);
extern void func_00209300(void *,void *);
void func_002093C8(char *obj,float *pos,float *out) {
 float delta[4] __attribute__((aligned(16)));
 float velocity[4] __attribute__((aligned(16)));
 float omega[4] __attribute__((aligned(16)));
 __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%2)"::"r"(pos),"r"(obj+16),"r"(delta):"memory");
 func_002092B8(obj,velocity);
 func_00209300(obj,omega);
 __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,0(%2)"::"r"(delta),"r"(omega),"r"(out):"memory");
 __asm__ volatile("lqc2 $vf11,0(%0)"::"r"(out):"memory");
 __asm__ volatile("lqc2 $vf12,0(%0)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%1)"::"r"(velocity),"r"(out):"memory");
}
