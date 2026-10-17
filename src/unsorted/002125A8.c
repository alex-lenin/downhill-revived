/* UNIT 0x002125A8 0x00212688 */
#include "spatial.h"
static inline void SubVectorXYZ(Vector4 *out, const Vector4 *a, const Vector4 *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(out), "r"(a), "r"(b) : "memory");
}

static inline void AddVectorXYZ(Vector4 *out, const Vector4 *a, const Vector4 *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(out), "r"(a), "r"(b) : "memory");
}

static inline void ApplyMatrix(Vector4 *out, const Matrix4 *m, const Vector4 *in) {
    __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)" : : "r"(out), "r"(m), "r"(in) : "memory");
}

void func_002125A8(SpatialReference *p, const Vector4 *input, Vector4 *output, float t) {
    Vector4 pos;
    Matrix4 matrix;
    float w;

    w = input->w;
    if (p->node) {
        SubVectorXYZ(output, input, &p->node->matrix->row[3]);
        ApplyMatrix(output, p->node->matrix, output);
    } else if (p->actor) {
        func_00209460(p->actor, &pos, &matrix, t);
        ApplyMatrix(output, &matrix, input);
        AddVectorXYZ(output, output, &pos);
    }
    output->w = w;
}
