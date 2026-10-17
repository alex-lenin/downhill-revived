/* UNIT 0x00130FE0 0x00131198 */
typedef float Vector[4] __attribute__((aligned(16)));
extern void func_00134D70(void *,float *,void *);
extern void func_00231A48(void *,float *);
#define SUB(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define ADD(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define SCALE(dst,a,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"f"(s) : "$2")
void func_00130FE0(char *obj, Vector *a, Vector *b, int which, float t) {
 Vector v0,v1,v2,out;
 float *a1=a[1], *b1=b[1], *a2=a[2], *b2=b[2];
 SUB(v2,b2,a2); SCALE(v2,v2,t); ADD(v2,v2,a2);
 SUB(v1,b1,a1); SCALE(v1,v1,t); ADD(v1,v1,a1);
 SUB(v0,b[0],a[0]); SCALE(v0,v0,t); ADD(v0,v0,a[0]);
 switch(which) {
 case 0: func_00134D70(obj+0x2e0,out,obj); SUB(out,out,v0); func_00231A48(obj,out); break;
 case 1: func_00134D70(obj+0x360,out,obj); SUB(out,out,v1); func_00231A48(obj,out); break;
 case 2: func_00134D70(obj+0x3e0,out,obj); SUB(out,out,v2); func_00231A48(obj,out); break;
 }
}
