#ifndef DHD_GROUND_PROBE_H
#define DHD_GROUND_PROBE_H
typedef unsigned int GroundProbeQuad __attribute__((mode(TI)));
typedef union GroundProbeVector { GroundProbeQuad q; float f[4]; } GroundProbeVector;
typedef char GroundProbeVector_size_check[sizeof(GroundProbeVector)==16 ? 1 : -1];
#if !defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
float func_00211980(GroundProbeVector *, GroundProbeVector *, int, float);
#ifdef __cplusplus
}
#endif
static inline float ProbeGroundLegacy(const void *p, void *normal, int flags, float height) {
    return func_00211980((GroundProbeVector *)p,(GroundProbeVector *)normal,flags,height);
}
static inline float ProbeGroundHeightFirst(const void *p, void *normal, float height, int flags) {
    return func_00211980((GroundProbeVector *)p,(GroundProbeVector *)normal,flags,height);
}
static inline float ProbeGroundHeightBetween(const void *p, float height, void *normal, int flags) {
    return func_00211980((GroundProbeVector *)p,(GroundProbeVector *)normal,flags,height);
}
#endif
#endif
