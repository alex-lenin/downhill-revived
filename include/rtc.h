#ifndef DHD_RTC_H
#define DHD_RTC_H
/* CDVD real-time clock, BCD bytes in retail SDK order. */
typedef struct RtcClock { unsigned char stat, second, minute, hour, pad, day, month, year; } RtcClock;
typedef char RtcClock_size_check[sizeof(RtcClock) == 8 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int func_00248058(RtcClock *);
int func_00193B00(RtcClock *);
#if defined(__mips__)
void ReadRtcDiscard(void *) __asm__("func_00193B00");
void ReadRtcLegacy(void *, int) __asm__("func_00193B00");
#else
static inline int ReadRtcDiscard(void *out) { return func_00193B00((RtcClock *)out); }
static inline int ReadRtcLegacy(void *out, int unused) { return func_00193B00((RtcClock *)out); }
#endif
#ifdef __cplusplus
}
#endif
#endif
