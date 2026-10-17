/* UNIT 0x00232A80 0x00232AD0 */
void func_00232A80(char *obj, float *out, float *pos) {
 float diff[4] __attribute__((aligned(16)));
 __asm__ volatile("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,0(%2)" : : "r"(pos),"r"(obj+0x50),"r"(diff) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	vopmula.xyz ACC,$vf11,$vf12
	vopmsub.xyz $vf11,$vf12,$vf11
	sqc2 $vf11,0(%2)" : : "r"(obj+0xd0),"r"(diff),"r"(out) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,0(%0)" : : "r"(out),"r"(obj+0xc0) : "memory");
}
