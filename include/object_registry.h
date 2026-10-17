#ifndef DHD_OBJECT_REGISTRY_H
#define DHD_OBJECT_REGISTRY_H
#ifdef __cplusplus
extern "C" {
#endif
extern void *D_003B7758[];
void func_0017D388(void);
int func_0017D3A0(void *object);
void *func_0017D3D0(int index);
#ifdef __cplusplus
}
#endif

/* Extra resource setup and short return views survive only for matching EE
   call schedules. Native bridges invoke the one-argument pointer API. */
#if defined(__mips__)
#ifdef __cplusplus
extern "C" {
#endif
int RegisterObjectRetail(void *, unsigned int) asm("func_0017D3A0");
short RegisterObjectShortRetail(void *, unsigned int) asm("func_0017D3A0");
#ifdef __cplusplus
}
#endif
#else
static inline int RegisterObjectRetail(void *object, unsigned int ignoredResource) {
    return func_0017D3A0(object);
}
static inline short RegisterObjectShortRetail(void *object, unsigned int ignoredResource) {
    return (short)func_0017D3A0(object);
}
#endif
#endif
