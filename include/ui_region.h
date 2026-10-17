#ifndef DHD_UI_REGION_H
#define DHD_UI_REGION_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall0019C868(int, int, int, int, int, float, float, float, float) __asm__("func_0019C868");
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
void SharedCall0020ED30(int, int, int, int, float, float, float, float, float) __asm__("func_0020ED30");
#ifdef __cplusplus
}
#endif
static inline void Forward0019C868_0(const void * p0, float p1, float p2, float p3, float p4, int p5, int p6, int p7, int p8) { SharedCall0019C868((int)p0, (int)p5, (int)p6, (int)p7, (int)p8, (float)p1, (float)p2, (float)p3, (float)p4); }
static inline void Forward0019C868_1(const void * p0, int p1, float p2, float p3, float p4, float p5, int p6, int p7, int p8) { SharedCall0019C868((int)p0, (int)p1, (int)p6, (int)p7, (int)p8, (float)p2, (float)p3, (float)p4, (float)p5); }
static inline void Forward0020ED30_2(int p0, float p1, float p2, float p3, float p4, float p5, int p6, int p7, int p8) { SharedCall0020ED30((int)p0, (int)p6, (int)p7, (int)p8, (float)p1, (float)p2, (float)p3, (float)p4, (float)p5); }
#endif
#endif
