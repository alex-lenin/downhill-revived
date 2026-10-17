/* UNIT 0x00232AD0 0x00232B08 */
typedef unsigned int u128 __attribute__((mode(TI)));
void func_00232AD0(char *obj, u128 *in) {
 float scale;
 *(volatile u128 *)(obj+0xc0)=*in;
 __asm__ volatile("" : : : "memory");
 scale=1.0f/ *(volatile float *)obj;
 __asm__ volatile("lqc2 $vf11,0(%1)
	mfc1 $2,%2
	qmtc2 $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0" : "=m"(*(float (*)[4])(obj+0x60)) : "r"(in),"f"(scale) : "$2","memory");
}
