#ifndef DHD_FORMAT_H
#define DHD_FORMAT_H
#ifdef __cplusplus
extern "C" {
#endif
/* Writes hours:minutes:seconds; returns the character count. */
int func_002238A8(int, char *);
#if defined(__mips__)
void FormatHmsDiscard(int secs, char *out) __asm__("func_002238A8");
#else
static inline void FormatHmsDiscard(int secs, char *out) { func_002238A8(secs,out); }
#endif

#ifdef __cplusplus
}
#endif
#endif
