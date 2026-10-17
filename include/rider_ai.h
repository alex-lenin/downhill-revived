#ifndef DHD_RIDER_AI_H
#define DHD_RIDER_AI_H
#ifdef __cplusplus
extern "C" {
#endif
void func_00124B40(void *riderAi);
#if defined(__mips__)
void UpdateRiderAiLegacy(void *, const void *) __asm__("func_00124B40");
#else
static inline void UpdateRiderAiLegacy(void *riderAi, const void *unusedTarget) { func_00124B40(riderAi); }
#endif
#ifdef __cplusplus
}
#endif
#endif
