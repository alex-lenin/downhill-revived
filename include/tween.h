#ifndef DHD_TWEEN_H
#define DHD_TWEEN_H

/* 00208708 writes the interpolated value through target each timer tick. */
typedef struct FloatTween {
    float *target;
    float start, delta;
    int startTick, duration;
} FloatTween;

/* Known prefix of the rider's 0x110-spaced blend controllers. */
typedef struct RiderBlendController {
    char pad0[0x40];
    float weight;
    char pad44[0x3C];
    FloatTween tween;
} RiderBlendController;

typedef struct RiderSecondaryBlend {
    char pad0[0xF4];
    float weight;
    FloatTween tween;
} RiderSecondaryBlend;

typedef struct RiderBodyBlend {
    char pad0[0x40];
    float weight;
    char pad44[0x44];
    FloatTween tween;
} RiderBodyBlend;

typedef char FloatTween_size_check[sizeof(FloatTween) == 0x14 ? 1 : -1];
#define DHD_TWEEN_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
typedef char RiderBlend_weight_check[DHD_TWEEN_OFFSET(RiderBlendController, weight) == 0x40 ? 1 : -1];
typedef char RiderBlend_tween_check[DHD_TWEEN_OFFSET(RiderBlendController, tween) == 0x80 ? 1 : -1];
typedef char RiderSecondaryBlend_tween_check[DHD_TWEEN_OFFSET(RiderSecondaryBlend, tween) == 0xF8 ? 1 : -1];
typedef char RiderBodyBlend_tween_check[DHD_TWEEN_OFFSET(RiderBodyBlend, tween) == 0x88 ? 1 : -1];
#undef DHD_TWEEN_OFFSET

#ifdef __cplusplus
extern "C" {
#endif
int func_00208660(FloatTween *, float *, int, float, float);
int func_002086C8(FloatTween *, float *, int, float, float);
int func_00208708(FloatTween *);
void func_001B3170(RiderSecondaryBlend *, int, float);
void func_001AE630(RiderBodyBlend *, int, float);
int func_001B1698(RiderBlendController *, int frames, float weight);
#if defined(__mips__)
void SetSecondaryBlendWeightFirst(RiderSecondaryBlend *, float, int) __asm__("func_001B3170");
void SetSecondaryBlendFloatFirst(float, RiderSecondaryBlend *, int) __asm__("func_001B3170");
void SetBodyBlendWeightFirst(RiderBodyBlend *, float, int) __asm__("func_001AE630");
#else
static inline void SetSecondaryBlendWeightFirst(RiderSecondaryBlend *blend, float weight, int frames) {
    func_001B3170(blend, frames, weight);
}
static inline void SetSecondaryBlendFloatFirst(float weight, RiderSecondaryBlend *blend, int frames) {
    func_001B3170(blend, frames, weight);
}
static inline void SetBodyBlendWeightFirst(RiderBodyBlend *blend, float weight, int frames) {
    func_001AE630(blend, frames, weight);
}
#endif
/* Retain the retail argument setup and discarded-return schedules on EE. */
#if defined(__mips__)
int SetRiderBlendWeightFirst(RiderBlendController *, float, int) __asm__("func_001B1698");
void SetRiderBlendWeightFirstDiscard(RiderBlendController *, float, int) __asm__("func_001B1698");
void SetRiderBlendDiscard(RiderBlendController *, int, float) __asm__("func_001B1698");
#else
static inline int SetRiderBlendWeightFirst(RiderBlendController *blend, float weight, int frames) {
    return func_001B1698(blend, frames, weight);
}
static inline void SetRiderBlendWeightFirstDiscard(RiderBlendController *blend, float weight, int frames) {
    func_001B1698(blend, frames, weight);
}
static inline void SetRiderBlendDiscard(RiderBlendController *blend, int frames, float weight) {
    func_001B1698(blend, frames, weight);
}
#endif
#ifdef __cplusplus
}
#endif

#endif
