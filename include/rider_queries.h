#ifndef DHD_RIDER_QUERIES_H
#define DHD_RIDER_QUERIES_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
struct Shared0011B268_struct_World;
struct Vector4;
struct Rider;
#ifdef __cplusplus
extern "C" {
#endif
struct Rider * SharedCall0011B268(struct Shared0011B268_struct_World *, struct Vector4 *, float *, float, float, struct Vector4 *) __asm__("func_0011B268");
#ifdef __cplusplus
}
#endif
struct Shared0011C0D0_World;
#ifdef __cplusplus
extern "C" {
#endif
int SharedCall0011C0D0(struct Shared0011C0D0_World *, struct Vector4 *, int, float) __asm__("func_0011C0D0");
#ifdef __cplusplus
}
#endif
struct Shared0011C168_Game;
#ifdef __cplusplus
extern "C" {
#endif
struct Rider * SharedCall0011C168(struct Shared0011C168_Game *, float *, float *, float) __asm__("func_0011C168");
#ifdef __cplusplus
}
#endif
static inline int Forward0011C0D0_0(const void * p0, const void * p1, float p2, int p3) { return (int)SharedCall0011C0D0((struct Shared0011C0D0_World *)p0, (struct Vector4 *)p1, (int)p3, (float)p2); }
static inline int Forward0011C168_1(const void * p0, const void * p1, float p2, const void * p3) { return (int)SharedCall0011C168((struct Shared0011C168_Game *)p0, (float *)p1, (float *)p3, (float)p2); }
static inline int Forward0011B268_2(const void * p0, const void * p1, const void * p2, const void * p3, float p4, float p5) { return (int)SharedCall0011B268((struct Shared0011B268_struct_World *)p0, (struct Vector4 *)p1, (float *)p2, (float)p4, (float)p5, (struct Vector4 *)p3); }
#endif
#endif
