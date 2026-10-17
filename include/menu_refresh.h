#ifndef DHD_MENU_REFRESH_H
#define DHD_MENU_REFRESH_H
#ifdef __cplusplus
extern "C" {
#endif
void func_00187808(void);
#if defined(__mips__)
/* Matching callers supplied an unused context. */
void RefreshGlobalMenuLegacy(const void *) __asm__("func_00187808");
#else
static inline void RefreshGlobalMenuLegacy(const void *unused) { func_00187808(); }
#endif
#ifdef __cplusplus
}
#endif
#endif
