#ifndef DHD_TRACK_BINS_H
#define DHD_TRACK_BINS_H
typedef struct TrackBinNode { float pos[4]; char pad10[16]; } TrackBinNode;
typedef struct PathBins { float x0, x4, x8; TrackBinNode *nodes; int count; short bins[1024]; } PathBins;
#ifdef __cplusplus
extern "C" {
#endif
void func_00117A78(PathBins *, float *pos, short *outIndex, short *outPoint);
void func_00117A98(PathBins *, float *pos, short *outIndex, short *outPoint, float *outDistance);
#ifdef __cplusplus
}
#endif
#endif
