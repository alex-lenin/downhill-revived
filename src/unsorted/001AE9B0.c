/* UNIT 0x001AE9B0 0x001AEA30 */
#include "rider.h"
typedef Vector4 Vec4;

typedef Matrix4 Matrix;

typedef SceneNode Frame;

typedef Rider Body;

typedef RiderComponent Source;

typedef struct Attach {
    char pad0[0x10];
    Vec4 offset;
    char pad20[0xA0 - 0x20];
    Frame *frame;
} Attach;

static inline void ApplyRotation(Vec4 *v, Matrix *m, Vec4 *out)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v), "r"(m));
}

/* 64-byte matrix copy (as sceVu0CopyMatrix) */
static inline void CopyMatrix(Matrix *dst, Matrix *src)
{
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
        : "=m"(dst->row[0]), "=m"(dst->row[1]), "=m"(dst->row[2]), "=m"(dst->row[3])
        : "r"(src)
        : "$8", "$9", "$10", "$11");
}

static inline void AddVector(Vec4 *out, Vec4 *a, Vec4 *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a), "r"(b));
}

void func_001AE9B0(Attach *attach, Source *src)
{
    Frame *frame;

    attach->frame = src->node;
    frame = src->rider->bodyNode;
    CopyMatrix(&attach->frame->matrix, &frame->matrix);
    ApplyRotation(&attach->offset, &frame->matrix, &attach->frame->position);
    AddVector(&attach->frame->position, &frame->position, &attach->frame->position);
}
