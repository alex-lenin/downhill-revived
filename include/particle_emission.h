#ifndef DHD_PARTICLE_EMISSION_H
#define DHD_PARTICLE_EMISSION_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
struct Particle;
struct Shared00162690_struct_Params;
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall00162690(struct Particle **, int, struct Shared00162690_struct_Params *, float) __asm__("func_00162690");
#ifdef __cplusplus
}
#endif
struct Vector4;
struct Shared001B5DE8_World;
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall001B5DE8(struct Vector4 *, struct Vector4 *, int, struct Vector4 *, struct Vector4 *, unsigned int, struct Shared001B5DE8_World *, float) __asm__("func_001B5DE8");
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
int SharedCall0020C0A0(struct Vector4 *, struct Vector4 *, int, int, int, unsigned int, int, float, float, float, float) __asm__("func_0020C0A0");
#ifdef __cplusplus
}
#endif
static inline void Forward00162690_0(const void * p0, float p1, int p2, const void * p3) { SharedCall00162690((struct Particle **)p0, (int)p2, (struct Shared00162690_struct_Params *)p3, (float)p1); }
static inline void Forward001B5DE8_1(const void * p0, const void * p1, int p2, const void * p3, float p4, const void * p5, const void * p6, const void * p7) { SharedCall001B5DE8((struct Vector4 *)p0, (struct Vector4 *)p1, (int)p2, (struct Vector4 *)p3, (struct Vector4 *)p5, (unsigned int)p6, (struct Shared001B5DE8_World *)p7, (float)p4); }
static inline void Forward00162690_2(const void * p0, float p1, int p2, int p3) { SharedCall00162690((struct Particle **)p0, (int)p2, (struct Shared00162690_struct_Params *)p3, (float)p1); }
static inline void Forward001B5DE8_3(const void * p0, const void * p1, unsigned short p2, const void * p3, float p4, const void * p5, const void * p6, const void * p7) { SharedCall001B5DE8((struct Vector4 *)p0, (struct Vector4 *)p1, (int)p2, (struct Vector4 *)p3, (struct Vector4 *)p5, (unsigned int)p6, (struct Shared001B5DE8_World *)p7, (float)p4); }
static inline int Forward0020C0A0_4(const void * p0, const void * p1, unsigned int p2, unsigned int p3, float p4, float p5, float p6, float p7, int p8, int p9, int p10) { return (int)SharedCall0020C0A0((struct Vector4 *)p0, (struct Vector4 *)p1, (int)p2, (int)p3, (int)p8, (unsigned int)p9, (int)p10, (float)p4, (float)p5, (float)p6, (float)p7); }
#endif
#endif
