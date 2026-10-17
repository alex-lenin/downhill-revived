/* UNIT 0x00147760 0x00147920 */
#include "object_registry.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Mat {
    float m[4][4];
} Mat __attribute__((aligned(16)));



typedef struct Node {
    unsigned int flags; /* bits 0-5 type, bits 7-17 id */
    char pad4[0xC];
    union {
        struct {
            Vec offset;          /* type 3: xyz offset, next in w slot */
        } t3;
        struct {
            Mat rot;             /* type 4: rotation + translation (row 3) */
        } t4;
        struct Node *child;      /* type 6 */
    } u;
    struct Node *x50;
    char pad54[0x4];
    long x58;
    Mat x60;
    Vec xA0;
} Node;

typedef struct Ctx {
    char pad0[0x4];
    Node *x4;
    Node *x8;
    Node *xC;
} Ctx;

#define NEXT3(n) (*(Node **)&(n)->u.t3.offset.w)

extern void func_00221D58(Mat *m, int arg);
extern void func_00221930(Mat *out, Mat *a, Mat *b);


static inline void ApplyRot(Vec *out, Mat *m, Vec *v)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "lqc2 $vf13, 0x10(%2)\n"
        "lqc2 $vf14, 0x20(%2)\n"
        "vmulax.xyz ACC, $vf12, $vf11x\n"
        "vmadday.xyz ACC, $vf13, $vf11y\n"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(v), "r"(m));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, %1\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "m"(*a), "r"(b));
}

/* uses VU0 inline asm */
void func_00147760(Ctx *ctx, Node *node, int count, Vec *pos_in, int arg)
{
    Vec pos;
    Mat mat;
    Vec tmp;

    if ((node->flags & 0x3F) == 6) {
        ctx->x8 = node;
        pos = *pos_in;
        func_00221D58(&mat, arg);
        node = ctx->x8->u.child;
        while ((node->flags & 0x3F) == 3 || (node->flags & 0x3F) == 4) {
            if ((node->flags & 0x3F) == 3) {
                ApplyRot(&tmp, &mat, &node->u.t3.offset);
                VecAdd(&pos, &pos, &tmp);
                node = NEXT3(node);
            } else {
                ApplyRot(&tmp, &mat, (Vec *)node->u.t4.rot.m[3]);
                VecAdd(&pos, &pos, &tmp);
                func_00221930(&mat, &node->u.t4.rot, &mat);
                node = node->x50;
            }
        }
        ctx->xC = node;
        node->xA0 = pos;
        /* 64-byte matrix copy (as sceVu0CopyMatrix) */
        __asm__ volatile (
            "lq $8, 0x0(%4)
	"
            "lq $9, 0x10(%4)
	"
            "lq $10, 0x20(%4)
	"
            "lq $11, 0x30(%4)
	"
            "sq $8, %0
	"
            "sq $9, %1
	"
            "sq $10, %2
	"
            "sq $11, %3"
            : "=m"(ctx->xC->x60.m[0]), "=m"(ctx->xC->x60.m[1]), "=m"(ctx->xC->x60.m[2]), "=m"(ctx->xC->x60.m[3])
            : "r"(&mat)
            : "$8", "$9", "$10", "$11");
        ctx->xC->x58 |= 1L << 36;
    } else {
        ctx->x4 = node;
        node->flags = (node->flags & 0xFFFC007F) | ((RegisterObjectRetail((void *)(ctx), (unsigned int)(node)) & 0x7FF) << 7);
    }
}
