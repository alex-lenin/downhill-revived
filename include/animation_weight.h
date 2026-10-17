#ifndef DHD_ANIMATION_WEIGHT_H
#define DHD_ANIMATION_WEIGHT_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
struct AnimationHandle;
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall00203B68(const struct AnimationHandle *, int, float) __asm__("func_00203B68");
#ifdef __cplusplus
}
#endif
static inline void Forward00203B68_0(const void * p0, float p1, int p2) { SharedCall00203B68((const struct AnimationHandle *)p0, (int)p2, (float)p1); }
#endif
#endif
