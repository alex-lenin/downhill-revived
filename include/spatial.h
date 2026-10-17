#ifndef DHD_SPATIAL_H
#define DHD_SPATIAL_H

#include "vector.h"
#include "spatial_batch.h"

typedef struct SpatialFilterBits {
    unsigned id:16;
    unsigned flag1:1;
    unsigned flag2:1;
    unsigned flag3:1;
    unsigned reserved:13;
} SpatialFilterBits;

typedef struct SpatialFilter {
    unsigned int x0;
    union { unsigned short id; unsigned int flags; SpatialFilterBits bits; } x4;
} SpatialFilter;

typedef struct SpatialEntry {
    float minX, minY, minZ, maxX, maxY, maxZ;
    int x18;
    unsigned int flags;
} SpatialEntry;

/* DI keeps the EE 64-bit flag word 64 bits in the i386 header too. */
typedef long SpatialActorFlags __attribute__((mode(DI)));
typedef struct SpatialActor {
    SpatialActorFlags x0;
    char pad8[0x78];
    int x80;
    char pad84[0x38];
    int xBC;
} SpatialActor;
typedef struct SpatialLink {
    struct SpatialLink *next;
    SpatialActor *actor;
} SpatialLink;

typedef struct SpatialCellIterator {
    int id;
    char *base;
    unsigned int *cur, *end;
    SpatialEntry *entry;
    SpatialLink *node;
    SpatialFilter filter;
} SpatialCellIterator;

/* A pointer to a matrix pointer, plus an optional moving actor. */
typedef struct SpatialMatrixNode { Matrix4 *matrix; } SpatialMatrixNode;
/* Moving transform sampled by 00209460; this is a prefix, not an array stride. */
typedef struct SpatialMotion {
    char pad0[0x10];
    Vector4 x10;
    char pad20[0x20];
    Matrix4 x40;
    char pad80[0x10];
    Vector4 x90;
    char padA0[0x10];
    float xB0;
    char padB4[0xC];
    Matrix4 xC0;
} SpatialMotion;

typedef struct SpatialReference {
    SpatialMatrixNode *node;
    SpatialMotion *actor;
} SpatialReference;

typedef struct SpatialCollider {
    int x0;
    unsigned short x4, x6;
    union { unsigned short index; unsigned short *map; } x8;
    void *xC[5];
    char x20[1];
} SpatialCollider;

typedef struct SpatialIterator {
    Vector4 min, max;
    int x20, x24, x28, x2C, x30, x34;
    SpatialCellIterator cell;
    SpatialReference reference;
    SpatialCollider *collider;
    Matrix4 *matrix;
    unsigned short x68, x6A;
    char pad6C[4];
} SpatialIterator;

typedef struct SpatialHit {
    short x0;
    unsigned short x2, x4, x6;
    float x8;
    int xC;
    Vector4 x10, x20;
} SpatialHit;

typedef char SpatialFilter_size_check[sizeof(SpatialFilter) == 8 ? 1 : -1];
typedef char SpatialCellIterator_size_check[sizeof(SpatialCellIterator) == 0x20 ? 1 : -1];
typedef char SpatialIterator_size_check[sizeof(SpatialIterator) == 0x70 ? 1 : -1];
typedef char SpatialHit_size_check[sizeof(SpatialHit) == 0x30 ? 1 : -1];
#define DHD_SPATIAL_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
typedef char SpatialIterator_cell_check[DHD_SPATIAL_OFFSET(SpatialIterator, cell) == 0x38 ? 1 : -1];
typedef char SpatialIterator_reference_check[DHD_SPATIAL_OFFSET(SpatialIterator, reference) == 0x58 ? 1 : -1];
typedef char SpatialIterator_collider_check[DHD_SPATIAL_OFFSET(SpatialIterator, collider) == 0x60 ? 1 : -1];
typedef char SpatialIterator_tags_check[DHD_SPATIAL_OFFSET(SpatialIterator, x68) == 0x68 ? 1 : -1];
typedef char SpatialMotion_matrix_check[DHD_SPATIAL_OFFSET(SpatialMotion, x40) == 0x40 ? 1 : -1];
typedef char SpatialMotion_duration_check[DHD_SPATIAL_OFFSET(SpatialMotion, xB0) == 0xB0 ? 1 : -1];
typedef char SpatialMotion_reference_check[DHD_SPATIAL_OFFSET(SpatialMotion, xC0) == 0xC0 ? 1 : -1];
#undef DHD_SPATIAL_OFFSET

#ifdef __cplusplus
extern "C" {
#endif
int func_00214A18(Vector4 *, Vector4 *, SpatialHit *, int maxHits, float radius, float arg5, float arg6);
void func_002092B8(SpatialMotion *, Vector4 *);
void func_00209460(SpatialMotion *, Vector4 *, Matrix4 *, float);
void func_00212488(SpatialReference *, const Matrix4 *, Matrix4 *, float);
void func_00212500(SpatialReference *, const Vector4 *, Vector4 *, float);
void func_002125A8(SpatialReference *, const Vector4 *, Vector4 *, float);
void func_00212688(SpatialReference *, const Vector4 *, Vector4 *, float);
SpatialFilter *func_00210950(void);
SpatialCellIterator *func_00211D68(SpatialCellIterator *);
void func_00211DF0(SpatialCellIterator *, int, int);
void func_00211EC8(SpatialCellIterator *);
SpatialIterator *func_00212048(SpatialIterator *);
void func_00212098(SpatialIterator *, Vector4 *, Vector4 *, float);
int func_002121E0(SpatialIterator *);
void func_00212408(SpatialIterator *);
SpatialReference *func_00212738(SpatialReference *);
SpatialHit *func_00211780(SpatialHit *, int, SpatialHit *);
int func_00210A18(SpatialCollider *, SpatialReference *, void *, void *, float, float, float, SpatialHit *, int);
int func_00210CA8(SpatialCollider *, SpatialReference *, float, void *, void *, float, float, Matrix4 *, int, SpatialHit *, int);
/* Retain retail bank setup; native forwards the canonical hit/count/float order. */
#if defined(__mips__)
int QuerySpatialHitsFloatFirst(Vector4 *, Vector4 *, float, float, float, SpatialHit *, int) __asm__("func_00214A18");
int QuerySpatialHitsCountLast(Vector4 *, Vector4 *, SpatialHit *, float, float, float, int) __asm__("func_00214A18");
#else
static inline int QuerySpatialHitsFloatFirst(Vector4 *a, Vector4 *b, float radius, float lo, float hi, SpatialHit *hits, int count) {
    return func_00214A18(a,b,hits,count,radius,lo,hi);
}
static inline int QuerySpatialHitsCountLast(Vector4 *a, Vector4 *b, SpatialHit *hits, float radius, float lo, float hi, int count) {
    return func_00214A18(a,b,hits,count,radius,lo,hi);
}
#endif

#if defined(__mips__)
int QueryColliderSweepLegacy(SpatialCollider *, SpatialReference *, Vector4 *, Vector4 *, Matrix4 *, float, int, SpatialHit *, int, float, float) __asm__("func_00210CA8");
#else
static inline int QueryColliderSweepLegacy(SpatialCollider *c, SpatialReference *r, Vector4 *a, Vector4 *b, Matrix4 *m, float radius, int flags, SpatialHit *hits, int count, float lo, float hi) {
    return func_00210CA8(c,r,radius,a,b,lo,hi,m,flags,hits,count);
}
#endif
#ifdef __cplusplus
}
#endif

#endif
