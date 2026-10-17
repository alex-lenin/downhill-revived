#ifndef DHD_TRACK_PROJECTION_H
#define DHD_TRACK_PROJECTION_H
typedef float TrackProjectionAngles[3];
#if !defined(__mips__)
struct TrackProjectionState;
struct Vector4;
#ifdef __cplusplus
extern "C" {
#endif
int func_00119180(struct TrackProjectionState *, struct Vector4 *, struct Vector4 *, float *angles, int flags);
#ifdef __cplusplus
}
#endif
static inline void ProjectTrackDiscard(const void *track, const void *pos, const void *out, const void *angles, int flags) {
    func_00119180((struct TrackProjectionState *)track,(struct Vector4 *)pos,(struct Vector4 *)out,(float *)angles,flags);
}
static inline void ProjectTrackAddressDiscard(const void *track, const void *pos, const void *out, int angles, int flags) {
    func_00119180((struct TrackProjectionState *)track,(struct Vector4 *)pos,(struct Vector4 *)out,(float *)angles,flags);
}
#endif
#endif
