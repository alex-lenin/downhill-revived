/* UNIT 0x0015BE88 0x0015BEA0 */
typedef struct Vec4 { float x, y, z, w; } __attribute__((aligned(16))) Vec4;
struct Obj { char pad[0x150]; Vec4 x150; };

/* sceVu0CopyVector-style 128-bit copy (libvu0 inline asm) */
void func_0015BE88(struct Obj *obj, int unused, Vec4 *out)
{
    __asm__ volatile ("lq $2, 0x0(%0)\n\tsq $2, 0x0(%1)" : : "r"(&obj->x150), "r"(out) : "$2", "memory");
}
