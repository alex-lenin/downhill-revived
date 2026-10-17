#ifndef DHD_RACE_LIMIT_H
#define DHD_RACE_LIMIT_H
#ifdef __cplusplus
extern "C" {
#endif
int func_0014E9D0(void);
#if defined(__mips__)
/* Matching callers supplied an unused context. */
int RaceLimitReachedLegacy(const void *) __asm__("func_0014E9D0");
#else
static inline int RaceLimitReachedLegacy(const void *unused) { return func_0014E9D0(); }
#endif
#ifdef __cplusplus
}
#endif
#endif
