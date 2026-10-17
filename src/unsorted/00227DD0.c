/* UNIT 0x00227DD0 0x00227F10 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Params {char pad0[20]; short x14[4]; signed char x1C[4];} Params;
extern Params D_0027EB70,D_0027EB90;
extern float D_0027EB10[],D_0027EB40[];
void func_00227DD0(void) {
 float input[4] __attribute__((aligned(16)));
 float sine[4] __attribute__((aligned(16)));
 float cosine[4] __attribute__((aligned(16)));
 int i;
 for(i=0;i<4;i++) {
 input[0]=(float)D_0027EB70.x14[i]; input[1]=(float)D_0027EB70.x1C[i];
 input[2]=(float)D_0027EB90.x14[i]; input[3]=(float)D_0027EB90.x1C[i];
 __asm__ volatile("lqc2 $vf11,%0\n\tmfc1 $2,%1\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "+m"(input) : "f"(0.01745329238474369f) : "$2");
 __asm__ volatile("lqc2 $vf14,0(%0)
	vcallms 0x978
	nop
	qmfc2.i $2,$vf14
	sqc2 $vf15,0(%2)
	sq $2,0(%1)" : : "r"((long)input),"r"(sine),"r"(cosine) : "$2","memory");
 D_0027EB10[i]=sine[0]*cosine[1];
 D_0027EB10[i+4]=cosine[0]*cosine[1];
 D_0027EB10[i+8]=sine[1];
 D_0027EB40[i]=sine[2]*cosine[3];
 D_0027EB40[i+4]=cosine[2]*cosine[3];
 D_0027EB40[i+8]=sine[3];
 }
}
