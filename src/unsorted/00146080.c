/* UNIT 0x00146080 0x001460D0 */
int func_00146080(void *a,void *b,float radius) {
 float vec[4];
 float x,y,z;
 int result;
 __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%2)" : : "r"(a),"r"(b),"r"(vec) : "memory");
 x=vec[0]; y=vec[1]; z=vec[2];
 __asm__ volatile("mula.s %0, %0\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "+f"(x), "+f"(radius) : "f"(y),"f"(z));
 result=1;
 if (!(x<radius*radius)) result=0;
 return result;
}
