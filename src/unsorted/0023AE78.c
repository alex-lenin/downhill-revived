/* UNIT 0x0023AE78 0x0023AEB8 */
typedef struct Matrix { float m[4][4]; } __attribute__((aligned(16))) Matrix;

extern Matrix D_00769830[];

/* sceVu0CopyMatrix-style 4x4 matrix copy (libvu0 inline asm) */
void func_0023AE78(int index, Matrix *src)
{
    __asm__ volatile (
        "lq $8, 0x0(%1)\n\t"
        "lq $9, 0x10(%1)\n\t"
        "lq $10, 0x20(%1)\n\t"
        "lq $11, 0x30(%1)\n\t"
        "sq $8, 0x0(%0)\n\t"
        "sq $9, 0x10(%0)\n\t"
        "sq $10, 0x20(%0)\n\t"
        "sq $11, 0x30(%0)"
        : : "r"(&D_00769830[index]), "r"(src) : "$8", "$9", "$10", "$11", "memory");
}
