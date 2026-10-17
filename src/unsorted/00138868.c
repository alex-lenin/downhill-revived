/* UNIT 0x00138868 0x00138900 */
extern float D_003613D0[];
extern float D_003613E0[];
extern void func_00228090(void *,unsigned int,int,float,float);
void func_00138868(unsigned char *obj,float t) {
 float v[4];
 if(obj[0x31765]) {
  float scale=1.0f-t;
  /* SDK-style VU0 vector scaling and scaled addition. */
  __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)" : : "r"(v),"r"(D_003613D0),"f"(scale) : "$2","memory");
  __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,0(%0)" : : "r"(v),"r"(D_003613E0),"f"(t) : "$2","memory");
  func_00228090(v,0x8008ff08,0,2.5f,5.0f);
 }
}
