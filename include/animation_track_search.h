#ifndef DHD_ANIMATION_TRACK_SEARCH_H
#define DHD_ANIMATION_TRACK_SEARCH_H
/* Reviewed native contracts; EE scheduling views remain in the matching units. */
#if !defined(__mips__)
struct Shared00163C58_struct_Table;
#ifdef __cplusplus
extern "C" {
#endif
int SharedCall00163C58(struct Shared00163C58_struct_Table *, int, int, float) __asm__("func_00163C58");
#ifdef __cplusplus
}
#endif
static inline int Forward00163C58_0(const void * p0, float p1, int p2, int p3) { return (int)SharedCall00163C58((struct Shared00163C58_struct_Table *)p0, (int)p2, (int)p3, (float)p1); }
#endif
#endif
