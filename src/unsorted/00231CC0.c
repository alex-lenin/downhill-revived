/* UNIT 0x00231CC0 0x00231D48 */
extern void func_00231C88(void *, void *);
extern int func_00221888(void *, void *, void *);
int func_00231CC0(char *obj, void *a, void *b) {
 float cross[4] __attribute__((aligned(16)));
 float delta[4] __attribute__((aligned(16)));
 func_00231C88(obj,a);
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(delta) : "r"(b),"r"(obj+0x50) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vopmula.xyz ACC,$vf11,$vf12
	vopmsub.xyz $vf11,$vf12,$vf11
	sqc2 $vf11,%0" : "=m"(cross) : "r"(delta),"r"(a) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(*(float (*)[4])(obj+0x70)) : "r"(obj+0x70),"r"(cross) : "memory");
 return func_00221888(obj+0x80,obj+0x70,obj+0xd0);
}
