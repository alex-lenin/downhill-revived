#ifndef DHD_MOTION_SAMPLE_H
#define DHD_MOTION_SAMPLE_H
#ifdef __cplusplus
extern "C" {
#endif
/* Returns the selected key index; writes the interpolated fraction and values. */
int func_00167A00(void *base, int track, float *fraction, float *value, float *extra, float time);
#if defined(__mips__)
void SampleMotionDiscard(void *, int, float *, void *, float *, float) __asm__("func_00167A00");
#else
static inline int SampleMotionDiscard(void *base, int track, float *fraction, void *value, float *extra, float time) { return func_00167A00(base,track,fraction,(float *)value,extra,time); }
#endif
#ifdef __cplusplus
}
#endif
#endif
