#ifndef DHD_SPATIAL_BATCH_H
#define DHD_SPATIAL_BATCH_H
struct Vector4;
struct Matrix4;
struct SpatialHit;
typedef struct SpatialBatchQuery {
    struct Vector4 *x0, *x4;
    float x8, xC, x10;
    struct Matrix4 *x14;
    int x18, x1C;
    struct SpatialHit *x20;
    int x24;
} SpatialBatchQuery;
typedef char SpatialBatchQuery_size_check[sizeof(SpatialBatchQuery)==0x28 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
int func_00214BA0(SpatialBatchQuery *, int);
#if defined(__mips__)
void QuerySpatialBatchDiscard(SpatialBatchQuery *, int) __asm__("func_00214BA0");
#else
static inline void QuerySpatialBatchDiscard(SpatialBatchQuery *records, int count) { func_00214BA0(records,count); }
#endif
#ifdef __cplusplus
}
#endif
#endif
