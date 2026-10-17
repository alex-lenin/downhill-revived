/* UNIT 0x00122098 0x001220B0 */
typedef struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4;

struct Obj {
    char pad[0x170];
    Vec4 x170;
};

/* sceVu0CopyVector-style 128-bit copy (libvu0 inline asm) */
void func_00122098(struct Obj *obj, int unused, Vec4 *out)
{
    __asm__ volatile ("lq $2, 0x0(%0)\n\tsq $2, 0x0(%1)" : : "r"(&obj->x170), "r"(out) : "$2", "memory");
}
