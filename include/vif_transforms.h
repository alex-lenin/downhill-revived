#ifndef DHD_VIF_TRANSFORMS_H
#define DHD_VIF_TRANSFORMS_H
#include "vector.h"
typedef struct VifTransformSource { char pad0[0x10]; float x10,x14,x18; char pad1C[0x24]; float matrix[16] __attribute__((aligned(16))); } VifTransformSource;
typedef struct VifTransformState { char pad0[0x20]; float x20,x24,x28; char pad2C[0x44]; float x70[16] __attribute__((aligned(16))); float xB0[16] __attribute__((aligned(16))); } VifTransformState;
typedef char VifTransformSource_size_check[sizeof(VifTransformSource) == 0x80 ? 1 : -1];
typedef char VifTransformState_size_check[sizeof(VifTransformState) == 0xF0 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int func_00216E28(VifTransformSource *, VifTransformState *, Vector4 *);
#if defined(__mips__)
void PrepareVifTransformsDiscard(void *, void *, void *) __asm__("func_00216E28");
void PrepareVifTransformsAddress(int, void *, void *) __asm__("func_00216E28");
#else
static inline int PrepareVifTransformsDiscard(void *source, void *state, void *out) { return func_00216E28((VifTransformSource *)source,(VifTransformState *)state,(Vector4 *)out); }
static inline int PrepareVifTransformsAddress(int source, void *state, void *out) { return PrepareVifTransformsDiscard((void *)source,state,out); }
#endif
#ifdef __cplusplus
}
#endif
#endif
