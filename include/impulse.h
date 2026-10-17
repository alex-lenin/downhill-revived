#ifndef DHD_IMPULSE_H
#define DHD_IMPULSE_H
#include "vector.h"
/* Linear impulse at +60, moment at +70, and their scaled/local outputs. */
typedef struct ImpulseBody {
    float inverseMass;
    char pad4[0x30];
    const Vector4 *translation; /* 0x34: world position of the body origin */
    const Matrix4 *orientation; /* 0x38: rotation applied to body-local vectors */
    char pad3C[0x14];
    Vector4 position, linearImpulse, moment;
    Matrix4 inverseInertia;
    Vector4 velocityDelta, localMoment;
} ImpulseBody;
#ifdef __cplusplus
extern "C" {
#endif
void func_00231C88(ImpulseBody *, Vector4 *);
void func_00231CC0(ImpulseBody *, Vector4 *, Vector4 *);
/* Rotates a body-local impulse (only xyz reach the result) by orientation. */
void func_00231D48(ImpulseBody *, const Vector4 *);
/* out = orientation * in + translation (xyz); out's w lane is kept, in's is not read. */
void func_001668C8(ImpulseBody *, Vector4 *out, const Vector4 *in);
#if defined(__mips__)
void ApplyImpulseLegacySetup(ImpulseBody *, Vector4 *, int, void *) __asm__("func_00231C88");
#else
static inline void ApplyImpulseLegacySetup(ImpulseBody *body, Vector4 *impulse, int ignored, void *source) {
    func_00231C88(body, impulse);
}
#endif

#ifdef __cplusplus
}
#endif
#endif
