#ifndef DHD_SPATIAL_PRIMITIVES_H
#define DHD_SPATIAL_PRIMITIVES_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
struct Vector4;
struct Shared00211390_Obj;
#ifdef __cplusplus
extern "C" {
#endif
int SharedCall00211390(struct Vector4 *, unsigned int, unsigned int, struct Shared00211390_Obj **, int, float) __asm__("func_00211390");
#ifdef __cplusplus
}
#endif
struct Shared00211BE0_Hit;
#ifdef __cplusplus
extern "C" {
#endif
int SharedCall00211BE0(struct Vector4 *, struct Shared00211BE0_Hit *, float, float) __asm__("func_00211BE0");
#ifdef __cplusplus
}
#endif
struct Shared00213098_Hit;
#ifdef __cplusplus
extern "C" {
#endif
int SharedCall00213098(struct Vector4 *, struct Vector4 *, struct Vector4 *, struct Vector4 *, struct Shared00213098_Hit *, float, float, float) __asm__("func_00213098");
#ifdef __cplusplus
}
#endif
static inline int Forward00211390_0(const void * p0, int p1, int p2, const void * p3, float p4, int p5) { return (int)SharedCall00211390((struct Vector4 *)p0, (unsigned int)p1, (unsigned int)p2, (struct Shared00211390_Obj **)p3, (int)p5, (float)p4); }
static inline int Forward00211390_1(const void * p0, float p1, int p2, int p3, const void * p4, int p5) { return (int)SharedCall00211390((struct Vector4 *)p0, (unsigned int)p2, (unsigned int)p3, (struct Shared00211390_Obj **)p4, (int)p5, (float)p1); }
static inline int Forward00211BE0_2(const void * p0, float p1, const void * p2, float p3) { return (int)SharedCall00211BE0((struct Vector4 *)p0, (struct Shared00211BE0_Hit *)p2, (float)p1, (float)p3); }
static inline int Forward00213098_3(const void * p0, const void * p1, const void * p2, float p3, float p4, const void * p5, float p6, const void * p7) { return (int)SharedCall00213098((struct Vector4 *)p0, (struct Vector4 *)p1, (struct Vector4 *)p2, (struct Vector4 *)p5, (struct Shared00213098_Hit *)p7, (float)p3, (float)p4, (float)p6); }
#endif
#endif
