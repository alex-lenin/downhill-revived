#ifndef DHD_LOAD_QUEUE_H
#define DHD_LOAD_QUEUE_H
#ifdef __cplusplus
extern "C" {
#endif
void func_00230F60(void);
void func_00231230(void);
#if defined(__mips__)
void ProcessLoadQueueLegacy(const void *) __asm__("func_00230F60");
#else
static inline void ProcessLoadQueueLegacy(const void *unused) { func_00230F60(); }
#endif
#ifdef __cplusplus
}
#endif
#endif
