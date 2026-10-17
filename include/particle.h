#ifndef DHD_PARTICLE_H
#define DHD_PARTICLE_H

#include "vector.h"
typedef unsigned int ParticleQuad __attribute__((mode(TI)));
typedef struct ParticleQuadMatrix { ParticleQuad row[4]; } ParticleQuadMatrix;

/* 0x2860 stride of D_0067E180; quadword views retain explicit EE copies. */
typedef struct Particle {
    char pad0[0x2600];
    ParticleQuad x2600;
    char pad2610[0x20];
    ParticleQuad x2630;
    char pad2640[0x40];
    ParticleQuad x2680, x2690;
    char pad26A0[0x10];
    Matrix4 matrix;
    char pad26F0[0x10];
    ParticleQuad x2700;
    void *x2710, *x2714;
    char pad2718[8];
    int x2720;
    short x2724, x2726;
    char pad2728[0xC];
    ParticleQuad *x2734;
    Matrix4 *x2738;
    char pad273C[0x32];
    unsigned short x276E;
    char pad2770[0x50];
    int x27C0;
    char pad27C4[0x60];
    int x2824, x2828;
    char pad282C[4];
    unsigned int *x2830, *x2834, *x2838;
    char pad283C[0xC];
    void *x2848;
    int x284C;
    int x2850, x2854;
} Particle;

typedef char Particle_size_check[sizeof(Particle) == 0x2860 ? 1 : -1];
#define DHD_PARTICLE_OFFSET(field) ((unsigned int)&((Particle *)0)->field)
typedef char Particle_matrix_check[DHD_PARTICLE_OFFSET(matrix) == 0x26B0 ? 1 : -1];
typedef char Particle_direction_check[DHD_PARTICLE_OFFSET(x2734) == 0x2734 ? 1 : -1];
typedef char Particle_packet_check[DHD_PARTICLE_OFFSET(x2830) == 0x2830 ? 1 : -1];
#undef DHD_PARTICLE_OFFSET

#ifdef __cplusplus
extern "C" {
#endif
int func_0022CDC0(Vector4 *, int, Matrix4 *, int, float, Vector4 *);
int func_0022CFB8(Vector4 *, void *, int, Vector4 *, float, Matrix4 *, int, int);
/* Scale-first retail forms preserve setup while native uses the shared spawners. */
#if defined(__mips__)
int SpawnParticleScaleBeforeEffect(Vector4 *, int, Matrix4 *, float, int, Vector4 *) __asm__("func_0022CDC0");
int SpawnParticleScaleBeforeMatrix(Vector4 *, int, float, Matrix4 *, int, Vector4 *) __asm__("func_0022CDC0");
int SpawnParticleScaleBeforeDirection(Vector4 *, void *, int, float, Vector4 *, Matrix4 *, int, int) __asm__("func_0022CFB8");
#else
static inline int SpawnParticleScaleBeforeEffect(Vector4 *v, int id, Matrix4 *m, float scale, int effect, Vector4 *direction) {
    return func_0022CDC0(v,id,m,effect,scale,direction);
}
static inline int SpawnParticleScaleBeforeMatrix(Vector4 *v, int id, float scale, Matrix4 *m, int effect, Vector4 *direction) {
    return func_0022CDC0(v,id,m,effect,scale,direction);
}
static inline int SpawnParticleScaleBeforeDirection(Vector4 *v, void *parent, int id, float scale, Vector4 *direction, Matrix4 *m, int a, int b) {
    return func_0022CFB8(v,parent,id,direction,scale,m,a,b);
}
#endif

#ifdef __cplusplus
}
#endif

#endif
