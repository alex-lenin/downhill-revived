#ifndef DHD_ANIMATION_QUERY_H
#define DHD_ANIMATION_QUERY_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
float SharedCall00206E48(unsigned short *, int, float) __asm__("func_00206E48");
#ifdef __cplusplus
}
#endif
static inline float Forward00206E48_0(float p0, const void * p1, const void * p2) { return (float)SharedCall00206E48((unsigned short *)p1, (int)p2, (float)p0); }
#endif
#endif
