#ifndef DHD_MOTION_INTERPOLATION_H
#define DHD_MOTION_INTERPOLATION_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
struct Shared0012D660_struct_Obj;
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall0012D660(struct Shared0012D660_struct_Obj *, int, float) __asm__("func_0012D660");
#ifdef __cplusplus
}
#endif
struct Shared001DD218_Obj;
struct Vector4;
struct Shared001DD218_Target;
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall001DD218(struct Shared001DD218_Obj *, struct Vector4 *, struct Shared001DD218_Target *, float, float) __asm__("func_001DD218");
#ifdef __cplusplus
}
#endif
static inline void Forward001DD218_0(const void * p0, float p1, float p2, const void * p3, const void * p4) { SharedCall001DD218((struct Shared001DD218_Obj *)p0, (struct Vector4 *)p3, (struct Shared001DD218_Target *)p4, (float)p1, (float)p2); }
static inline void Forward0012D660_1(const void * p0, float p1, int p2) { SharedCall0012D660((struct Shared0012D660_struct_Obj *)p0, (int)p2, (float)p1); }
#endif
#endif
