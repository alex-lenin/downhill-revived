/* UNIT 0x0023AEB8 0x0023AF18 */
typedef unsigned int Q __attribute__((mode(TI)));
typedef struct Vec { float f[4]; } __attribute__((aligned(16))) Vec;
extern Vec D_007684B0[], D_0076E630[];
#define COPY_VECTOR(dst, src) \
 __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_0023AEB8(int index) {
 int i;
 for (i = index * 26; i < index * 26 + 26; i++) {
  COPY_VECTOR(D_0076E630[i], &D_007684B0[i]);
 }
}
