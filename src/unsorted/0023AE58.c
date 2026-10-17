/* UNIT 0x0023AE58 0x0023AE78 */
typedef float Vec[4];
extern Vec D_007684B0[];

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_0023AE58(int index, Vec src) {
    COPY_VECTOR(D_007684B0[index], src);
}
