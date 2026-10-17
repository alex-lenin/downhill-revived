#ifndef DHD_MENU_FINALIZE_H
#define DHD_MENU_FINALIZE_H
#ifdef __cplusplus
extern "C" {
#endif
void func_0018F278(void);
#if defined(__mips__)
/* Matching callers supplied an unused context. */
void FinalizeMenuLegacy(const void *) __asm__("func_0018F278");
#else
static inline void FinalizeMenuLegacy(const void *unused) { func_0018F278(); }
#endif
#ifdef __cplusplus
}
#endif
#endif
