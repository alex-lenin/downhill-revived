#ifndef DHD_RENDER_WAIT_H
#define DHD_RENDER_WAIT_H
#ifdef __cplusplus
extern "C" {
#endif
void func_001B4618(void);
#if defined(__mips__)
/* Matching callers supplied an unused context. */
void WaitForRenderLegacy(const void *) __asm__("func_001B4618");
#else
static inline void WaitForRenderLegacy(const void *unused) { func_001B4618(); }
#endif
#ifdef __cplusplus
}
#endif
#endif
