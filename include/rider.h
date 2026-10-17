#ifndef DHD_RIDER_H
#define DHD_RIDER_H

#include "vector.h"
#include "animation.h"

#include "scene.h"

typedef struct Rider Rider;
typedef struct RiderComponent {
    char pad0[0x14];
    /* Slot 0 plus the 253 outputs filled by 001AC418; next object is +FF4. */
    AnimationHandle handles[254];
    char xFF4[0x60];
    char x1054[0x16D0 - 0x1054];
    Rider *rider;
    SceneNode *node;
    char pad16D8[0x1D60 - 0x16D8];
    Vector4 x1D60;
    char pad1D70[0x1E70 - 0x1D70];
    Vector4 x1E70;
    char pad1E80[0x211C - 0x1E80];
    float x211C;
    char pad2120[0x2160 - 0x2120];
    unsigned char x2160;
    char pad2161[3];
    int x2164;
} RiderComponent;

typedef struct RiderPhysics {
    Rider *owner;
    char pad4[0x34];
    SceneNode *node;
    char pad3C[0x7C - 0x3C];
    int x7C;
    char pad80[0x40];
    Vector4 velocity;
    char padD0[0x160 - 0xD0];
    int x160;
    char pad164[0x404 - 0x164];
    int x404;
    char pad408[0x38];
    SceneNodeFlags flags;
    char pad448[0x4A4 - 0x448];
    short x4A4;
    char pad4A6[0xA];
} RiderPhysics;

typedef struct RiderSub6610 {
    char pad0[4];
    int x4;
    char pad8[4];
    float xC;
    char pad10[0x14];
    float x24;
    char pad28[0x10E6 - 0x28];
    unsigned char x10E6;
} RiderSub6610;

typedef struct RiderFade {
    char pad0[4];
    float x4;
    char pad8[4];
    float xC;
} RiderFade;

/* Known fields with padding to the 0x7BD0 stride proven by rider array users. */
struct Rider {
    /* 0012BF88 fills indices 1..253; slot 0 is retained. */
    AnimationHandle handles[254];
    char padFE0[0x1190 - 0xFE0];
    int x1190, x1194;
    char pad1198[8];
    int x11A0, x11A4;
    char pad11A8[0x1258 - 0x11A8];
    int x1258, x125C;
    char pad1260[8];
    int x1268, x126C;
    char pad1270[0x13D7 - 0x1270];
    unsigned char x13D7, x13D8;
    char pad13D9[2];
    unsigned char x13DB, x13DC;
    char pad13DD[0x1E40 - 0x13DD];
    RiderComponent component;
    char pad3FB0[0x40C0 - 0x3FB0];
    RiderPhysics physics;
    char pad4570[0x46D0 - 0x4570];
    char x46D0[0x48B0 - 0x46D0];
    char x48B0[0x4D28 - 0x48B0];
    float x4D28;
    char pad4D2C[0x4D40 - 0x4D2C];
    RiderFade x4D40;
    char pad4D50[0x621C - 0x4D50];
    char x621C[0x6390 - 0x621C];
    int x6390;
    char pad6394[0x6610 - 0x6394];
    RiderSub6610 sub6610;
    char pad76F8[0x78D4 - 0x76F8];
    void *x78D4;
    char pad78D8[0x7908 - 0x78D8];
    int x7908;
    char pad790C[0x792C - 0x790C];
    SceneNode *bodyNode;
    SceneNode *x7930;
    int x7934;
    char pad7938[0x796C - 0x7938];
    int x796C, x7970, x7974, x7978;
    int riderIndex;
    char pad7980[4];
    int x7984, x7988;
    int x798C;
    char pad7990[4];
    int x7994;
    union { int x7998; unsigned short u7998; };
    char pad799C[0x7A3C - 0x799C];
    float x7A3C;
    char pad7A40[0x7A5C - 0x7A40];
    int x7A5C;
    char pad7A60[8];
    unsigned char x7A68;
    char pad7A69[0x7A78 - 0x7A69];
    float x7A78;
    char pad7A7C[0x7AC0 - 0x7A7C];
    float x7AC0;
    char pad7AC4[4];
    float x7AC8, x7ACC, x7AD0, x7AD4, x7AD8;
    char pad7ADC[0x7BD0 - 0x7ADC];
};

#define DHD_RIDER_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
typedef char Rider_body_check[DHD_RIDER_OFFSET(Rider, bodyNode) == 0x792C ? 1 : -1];
typedef char Rider_index_check[DHD_RIDER_OFFSET(Rider, riderIndex) == 0x797C ? 1 : -1];
typedef char RiderComponent_node_check[DHD_RIDER_OFFSET(RiderComponent, node) == 0x16D4 ? 1 : -1];
typedef char RiderPhysics_size_check[sizeof(RiderPhysics) == 0x4B0 ? 1 : -1];
typedef char Rider_size_check[sizeof(Rider) == 0x7BD0 ? 1 : -1];
typedef char Rider_physics_check[DHD_RIDER_OFFSET(Rider, physics) == 0x40C0 ? 1 : -1];
typedef char Rider_velocity_check[DHD_RIDER_OFFSET(Rider, physics.velocity) == 0x4180 ? 1 : -1];
typedef char Rider_state_check[DHD_RIDER_OFFSET(Rider, physics.x160) == 0x4220 ? 1 : -1];
typedef char Rider_sub6610_check[DHD_RIDER_OFFSET(Rider, sub6610) == 0x6610 ? 1 : -1];
typedef char RiderFade_check[DHD_RIDER_OFFSET(Rider, x4D40) == 0x4D40 ? 1 : -1];
typedef char Rider_result_check[DHD_RIDER_OFFSET(Rider, x798C) == 0x798C ? 1 : -1];
typedef char Rider_speed_check[DHD_RIDER_OFFSET(Rider, x7AC0) == 0x7AC0 ? 1 : -1];
typedef char RiderComponent_handles_check[DHD_RIDER_OFFSET(RiderComponent, handles) == 0x14 ? 1 : -1];
typedef char RiderComponent_probe_check[DHD_RIDER_OFFSET(RiderComponent, x1D60) == 0x1D60 ? 1 : -1];
typedef char RiderComponent_probe2_check[DHD_RIDER_OFFSET(RiderComponent, x1E70) == 0x1E70 ? 1 : -1];
typedef char Rider_animation_flag_check[DHD_RIDER_OFFSET(Rider, x7A78) == 0x7A78 ? 1 : -1];
typedef char RiderComponent_end_handles_check[DHD_RIDER_OFFSET(RiderComponent, xFF4) == 0xFF4 ? 1 : -1];
typedef char RiderComponent_anim_weight_check[DHD_RIDER_OFFSET(RiderComponent, x211C) == 0x211C ? 1 : -1];
typedef char Rider_sub_flag_check[DHD_RIDER_OFFSET(Rider, sub6610.x10E6) == 0x76F6 ? 1 : -1];
typedef char Rider_object_check[DHD_RIDER_OFFSET(Rider, x78D4) == 0x78D4 ? 1 : -1];
typedef char RiderPhysics_flags_check[DHD_RIDER_OFFSET(RiderPhysics, flags) == 0x440 ? 1 : -1];
typedef char Rider_interaction_type_check[DHD_RIDER_OFFSET(Rider, x6390) == 0x6390 ? 1 : -1];
#undef DHD_RIDER_OFFSET

#ifdef __cplusplus
extern "C" {
#endif
int func_001B1C08(RiderComponent *);
void func_00113398(RiderPhysics *, Rider *, const Vector4 *, float distance);
int func_00113588(RiderPhysics *, Rider *, float distance);
void func_001AC418(RiderComponent *);
void func_001AE8A8(SceneNode *, Vector4 *, Vector4 *, Matrix4 *);
void func_0023AEB8(int);
#ifdef __cplusplus
}
#endif

#endif
