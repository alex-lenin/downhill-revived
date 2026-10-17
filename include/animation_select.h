#ifndef DHD_ANIMATION_SELECT_H
#define DHD_ANIMATION_SELECT_H
struct AnimationHandle;
typedef long AnimationLegacyAddress __attribute__((mode(DI)));
#ifdef __cplusplus
extern "C" {
#endif
void func_00202E38(struct AnimationHandle *, int mode, int flag, float duration);
void func_00203590(struct AnimationHandle *, int resource, int mode, int index);
#if defined(__mips__)
void SelectAnimationTimeFirst(struct AnimationHandle *, float, int, int) __asm__("func_00202E38");
void SelectAnimationLegacyAddress(AnimationLegacyAddress, int, int, float) __asm__("func_00202E38");
void SelectAnimationResourceLegacy(const void *, long, int, int) __asm__("func_00203590");
#else
static inline void SelectAnimationTimeFirst(struct AnimationHandle *handle, float duration, int mode, int flag) {
    func_00202E38(handle, mode, flag, duration);
}
static inline void SelectAnimationLegacyAddress(AnimationLegacyAddress address, int mode, int flag, float duration) {
    func_00202E38((struct AnimationHandle *)address, mode, flag, duration);
}
static inline void SelectAnimationResourceLegacy(const void *handle, int resource, int mode, int index) {
    func_00203590((struct AnimationHandle *)handle, resource, mode, index);
}
#endif
#ifdef __cplusplus
}
#endif
#endif
