#ifndef DHD_VECTOR_BLEND_H
#define DHD_VECTOR_BLEND_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall00223578(float *, float *, float) __asm__("func_00223578");
#ifdef __cplusplus
}
#endif
static inline void Forward00223578_0(float p0, const void * p1, const void * p2) { SharedCall00223578((float *)p1, (float *)p2, (float)p0); }
#endif
#endif
