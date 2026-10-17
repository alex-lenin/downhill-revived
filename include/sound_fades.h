#ifndef DHD_SOUND_FADES_H
#define DHD_SOUND_FADES_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall001CF5C8(float, int *, void *, int) __asm__("func_001CF5C8");
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall001D3D20(unsigned int *, void *, float, float) __asm__("func_001D3D20");
#ifdef __cplusplus
}
#endif
static inline void Forward001D3D20_0(float p0, const void * p1, float p2, const void * p3) { SharedCall001D3D20((unsigned int *)p1, (void *)p3, (float)p0, (float)p2); }
static inline void Forward001CF5C8_1(const void * p0, const void * p1, int p2, float p3) { SharedCall001CF5C8((float)p3, (int *)p0, (void *)p1, (int)p2); }
#endif
#endif
