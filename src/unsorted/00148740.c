/* UNIT 0x00148740 0x001487D8 */
extern void func_002226C8(void *, float *);
void func_00148740(char *obj) {
 if (*(int *)(obj + 0x348) == 0) {
  float *dir = (float *)(obj + 0xc0); float x,y,z,len,inv;
  char *pos = *(char **)(*(char **)(obj + 0x2b4) + 0x792c) + 0x10;
  __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0xc0(%2)" : : "r"(pos), "r"(obj+0x80), "r"(obj) : "memory");
  x=dir[0]; y=dir[1]; z=dir[2];
  __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x), "f"(y), "f"(z));
  __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
  dir[0]=x*inv; dir[1]=y*inv; dir[2]=z*inv;
  func_002226C8(obj,dir);
 }
}
