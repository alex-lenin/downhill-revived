#ifndef DHD_ANIMATION_H
#define DHD_ANIMATION_H
#include "animation_select.h"

/* Shared prefix layouts used by the 0x203xxx animation/parameter API. */
typedef struct AnimationRange {
    int x0;
    float x4, x8;
} AnimationRange;

typedef struct AnimationRangeTable {
    unsigned short x0, x2;
    char pad4[0xC];
    AnimationRange *entries[1];
} AnimationRangeTable;

typedef struct AnimationTrack {
    int x0;
    AnimationRange *x4;
    char pad8[0xC];
    int x14;
    float x18, x1C;
    char pad20[8];
    unsigned char x28;
    unsigned char x29;
    unsigned short x2A;
} AnimationTrack;

typedef struct AnimationTrackList {
    int x0, x4;
    AnimationTrack *x8;
} AnimationTrackList;

typedef struct AnimationOwner {
    unsigned int x0;
    char pad4[8];
    int xC;
    char *x10;
    char pad14[8];
    unsigned int x1C;
    AnimationRangeTable *x20;
    AnimationTrackList *x24;
} AnimationOwner;

typedef struct AnimationHandle {
    AnimationOwner *x0;
    AnimationTrack *x4;
    int x8;
    short xC, xE;
} AnimationHandle;

/* 0x64-byte animation dispatch/controller, including its +4 output payload. */
typedef struct AnimationController {
    AnimationHandle *x0;
    int x4;
    signed char x8;
    unsigned char x9;
    short xA;
    char padC[0x38];
    float x44;
    int x48;
    char pad4C[8];
    int x54, x58;
    float x5C, x60;
} AnimationController;
typedef char AnimationController_size_check[sizeof(AnimationController) == 0x64 ? 1 : -1];

typedef char AnimationHandle_size_check[sizeof(AnimationHandle) == 16 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
AnimationController *func_001AC780(AnimationController *, signed char, short);
void func_002036C8(AnimationHandle *, int, int, int, int);
void func_002037D0(AnimationHandle *, int, int, int);
void *func_002031E0(const AnimationHandle *, void *, int, short, int, float, float);
void func_00203380(const AnimationHandle *, void *);
void *func_002033E0(const AnimationHandle *, void *, int, short, float);
float func_00204280(const AnimationHandle *);
float func_002042F0(const AnimationHandle *, float);
float func_00203848(const AnimationHandle *);
void func_002038E0(const AnimationHandle *, int);
void func_002039B0(const AnimationHandle *, int);
void func_00203A80(const AnimationHandle *, int, float);
void func_00203B68(const AnimationHandle *, int, float);
float func_00203C28(const AnimationHandle *);
void func_00203CB8(const AnimationHandle *);
void func_00203D18(const AnimationHandle *);
void func_00203DC8(const AnimationHandle *, int);
void func_00203E10(const AnimationHandle *, float);
void func_00203EF0(const AnimationHandle *);
void func_00203F40(const AnimationHandle *, int);
void func_00203FE0(const AnimationHandle *, int);
void func_00204080(const AnimationHandle *, int);
int func_00204170(const AnimationHandle *);
float func_002041D0(const AnimationHandle *);
int func_00204230(const AnimationHandle *);
void func_00204450(const AnimationHandle *, unsigned short);
float func_00204770(AnimationHandle *);
void func_00206320(const AnimationHandle *);

typedef float (*AnimationHandler)(unsigned short *, int, float);
float func_002627A0(unsigned short *, int, float);
float func_002628A0(unsigned short *, int, float);
float func_00262B48(unsigned short *, int, float);
float func_00262C20(unsigned short *, int, float);
float func_00262D30(unsigned short *, int, float);
float func_00262FE0(unsigned short *, int, float);
float func_00263270(unsigned short *, int, float);
float func_00263320(unsigned short *, int, float);
float func_00206E40(unsigned short *, int, float);
float func_00263438(unsigned short *, int, float);
float func_00263578(unsigned short *, int, float);
float func_00263860(unsigned short *, int, float);
float func_002638D8(unsigned short *, int, float);
float func_002639D0(unsigned short *, int, float);

float func_00206E48(unsigned short *, int, float);
#ifdef __cplusplus
}
#endif

#endif
