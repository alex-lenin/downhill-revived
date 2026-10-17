#ifndef DHD_CAMERA_H
#define DHD_CAMERA_H
#include "vector.h"
struct RiderComponent;

typedef struct CameraParameter {
    char pad0[0x48];
    float value;
    int enabled;
} CameraParameter;
typedef char CameraParameter_size_check[sizeof(CameraParameter) == 0x50 ? 1 : -1];

/* Rider camera: transform/blend prefix plus four sound slots at +5C0. */
typedef struct RiderCamera {
    Vector4 x0, x10, x20, x30, x40, x50, x60, x70, x80, x90, xA0, xB0;
    struct RiderComponent *xC0;
    char padC4[0x2C];
    unsigned char xF0, xF1;
    char padF2[2];
    float xF4;
    char padF8[0x14];
    int x10C, x110;
    char pad114[0xC];
    CameraParameter parameters[13];
    float x530;
    char pad534[0x1C];
    float x550;
    char pad554[0x1C];
    float x570;
    char pad574[0x2C];
    float x5A0;
    char pad5A4[0xC];
    unsigned char x5B0;
    char pad5B1[0xF];
    char x5C0[0x450];
    struct RiderComponent *xA10;
    char padA14[0x3C];
    Vector4 xA50, xA60;
} RiderCamera;

typedef struct RiderCameraRig { char pad0[0x60]; Matrix4 matrix; void *xA0; } RiderCameraRig;
typedef struct CameraAngles { char pad0[0xA0]; float xA0, xA4, xA8; } CameraAngles;
typedef char RiderCamera_size_check[sizeof(RiderCamera) == 0xA70 ? 1 : -1];
#define DHD_CAMERA_OFFSET(field) ((unsigned int)&((RiderCamera *)0)->field)
typedef char RiderCamera_blend_check[DHD_CAMERA_OFFSET(xC0) == 0xC0 ? 1 : -1];
typedef char RiderCamera_slots_check[DHD_CAMERA_OFFSET(x5C0) == 0x5C0 ? 1 : -1];
typedef char RiderCamera_rider_check[DHD_CAMERA_OFFSET(xA10) == 0xA10 ? 1 : -1];
typedef char RiderCamera_vectors_check[DHD_CAMERA_OFFSET(xA60) == 0xA60 ? 1 : -1];
#undef DHD_CAMERA_OFFSET
#ifdef __cplusplus
extern "C" {
#endif
void func_001B2E50(CameraParameter *, float value);
void func_001B3028(RiderCamera *, int index, float value);
void func_001B0C10(RiderCamera *, struct RiderComponent *);
void func_001B0D38(RiderCamera *);
void func_001B0E00(RiderCamera *);
void func_001B0EA8(RiderCamera *);
void func_001B30B8(RiderCamera *, struct RiderComponent *);
void func_001B3108(RiderCamera *);
void func_001B3128(RiderCamera *);
void func_001B33A8(RiderCamera *);
void func_001B3858(RiderCamera *);
void func_001B3E98(RiderCamera *, Vector4 *);
void func_001AEB20(RiderCameraRig *, RiderCamera *, Vector4 *);
void func_001AEB88(RiderCameraRig *, RiderCamera *);
void func_001AE358(CameraAngles *, RiderCamera *);
#ifdef __cplusplus
}
#endif
#endif
