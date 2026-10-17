/* UNIT 0x00209248 0x002092B8 */
typedef struct { float x[4]; } Vec4;
typedef struct { Vec4 r[4]; } Mat4;
typedef struct { char pad0[0x10]; Vec4 x10; char pad20[0x20]; Mat4 x40; char pad80[0x10]; Vec4 x90; char padA0[0x10]; float xB0; char padB4[0xC]; Mat4 xC0; } Obj;

void func_00209248(Obj *p, Vec4 *pos, Mat4 *mat, float t)
{
    if (pos != 0) {
        Vec4 *src = &p->x10;
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, 0x90(%0)\n\tlq $2, 0x0(%2)\n\tsq $2, 0x10(%0)" : : "r"(p), "r"(src), "r"(pos) : "$2", "memory");
        p->xB0 = t;
    }
    if (mat != 0) {
        __asm__ volatile ("lq $8, 0x0(%0)\n\tlq $9, 0x10(%0)\n\tlq $10, 0x20(%0)\n\tlq $11, 0x30(%0)\n\tsq $8, 0xC0(%1)\n\tsq $9, 0xD0(%1)\n\tsq $10, 0xE0(%1)\n\tsq $11, 0xF0(%1)\n\tlq $8, 0x0(%2)\n\tlq $9, 0x10(%2)\n\tlq $10, 0x20(%2)\n\tlq $11, 0x30(%2)\n\tsq $8, 0x40(%1)\n\tsq $9, 0x10(%0)\n\tsq $10, 0x20(%0)\n\tsq $11, 0x30(%0)" : : "r"(&p->x40), "r"(p), "r"(mat) : "$8", "$9", "$10", "$11", "memory");
        p->xB0 = t;
    }
}
