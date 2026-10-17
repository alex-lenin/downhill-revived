/* UNIT 0x0010E918 0x0010E9B0 */
int func_0010E918(void *unused, float *a, float *b) {
 float diff = a[2] - b[2];
 float v[4] __attribute__((aligned(16)));
 float length;
 int result = 0;
 if (diff < 50.0f && diff > -100.0f) {
  __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v), "r"(a), "r"(b) : "memory");
  __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(v[0]), "f"(v[1]), "f"(v[2]));
  if (length < 75.0f) result = 1;
 }
 return result;
}
