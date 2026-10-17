#ifndef DHD_RIDER_EVENT_H
#define DHD_RIDER_EVENT_H
/* Embedded at rider +0x234; last word is the queue submission clock. */
typedef struct RiderEventState { int x0, x4, x8, xC, lastSubmission; } RiderEventState;
#ifdef __cplusplus
extern "C" {
#endif
int func_001BDDA0(RiderEventState *, void *);
#if defined(__mips__)
/* Discarded result view preserves retail caller register scheduling. */
void SubmitRiderEvent(void *, void *) __asm__("func_001BDDA0");
#else
static inline int SubmitRiderEvent(void *state, void *actor) { return func_001BDDA0((RiderEventState *)state, actor); }
#endif
#ifdef __cplusplus
}
#endif
#endif
