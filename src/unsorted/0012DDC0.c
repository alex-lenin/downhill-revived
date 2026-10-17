/* UNIT 0x0012DDC0 0x0012DE00 */
/* func_0012DDC0: copies the rider's cached pose (pos vector + rotation matrix)
   into the pose slot its body points at (sceVu0CopyVector / sceVu0CopyMatrix
   inline-asm forms; the asm operand order and clobbers are what the retail
   compiler's register allocation requires). */
typedef struct { float x, y, z, w; } Vec __attribute__((aligned(16)));

struct Rider {
    char pad0[0x4270];
    Vec x4270;              /* pos */
    Vec x4280[4];           /* rot matrix, 4 rows */
    char pad42c0[0x366c];
    Vec *x792C;             /* body pose slot (pos at +0x10, matrix at +0x40) */
};

void func_0012DDC0(struct Rider *rider)
{
    __asm__ volatile ("lq $2, 0x0(%0)\n\tsq $2, 0x10(%1)"
                      : : "r"(&rider->x4270), "r"(rider->x792C) : "$2", "$3", "memory");
    __asm__ volatile ("" : : : "memory");
    __asm__ volatile (
        "lq $8, 0x0(%0)\n\tlq $9, 0x10(%0)\n\tlq $10, 0x20(%0)\n\tlq $11, 0x30(%0)\n\t"
        "sq $8, 0x40(%1)\n\tsq $9, 0x50(%1)\n\tsq $10, 0x60(%1)\n\tsq $11, 0x70(%1)"
        : : "r"(&rider->x4280), "r"(rider->x792C) : "$8", "$9", "$10", "$11", "$2", "memory");
}
