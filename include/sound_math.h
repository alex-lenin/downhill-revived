#ifndef DHD_SOUND_MATH_H
#define DHD_SOUND_MATH_H
#ifdef __cplusplus
extern "C" {
#endif
/* Relative heading in degrees; heading is in radians, xy positions are vectors. */
int func_00208C60(float *listener, float *source, float heading);
#ifdef __cplusplus
}
#endif
#if !defined(__mips__)
/* Keep retail call setup in scheduling-sensitive units; native calls use the
 * canonical pointer/pointer/float contract above. */
static inline int SoundHeadingFirst(float heading, int listener, int source) {
    return func_00208C60((float *)listener, (float *)source, heading);
}
static inline int SoundHeadingBetween(const void *listener, float heading, const void *source) {
    return func_00208C60((float *)listener, (float *)source, heading);
}
#endif
#endif
