#ifndef DHD_PARTICLE_SPRAY_H
#define DHD_PARTICLE_SPRAY_H
typedef unsigned int ParticleSprayQuad __attribute__((mode(TI)));
typedef union ParticleSprayVector { ParticleSprayQuad q; float f[4]; int i[4]; signed char b[16]; } ParticleSprayVector __attribute__((aligned(16)));
typedef struct ParticleSprayEmitter {
    char pad0[0x2610];
    ParticleSprayVector x2610;          /* bounds min */
    ParticleSprayVector x2620;          /* bounds max */
    ParticleSprayVector x2630;          /* .i[3] = particle count */
    ParticleSprayVector x2640;
    ParticleSprayVector x2650;
    ParticleSprayVector x2660;
    char pad2670[0x20];
    ParticleSprayVector x2690;
    ParticleSprayVector x26A0;
    ParticleSprayVector x26B0;
    ParticleSprayVector x26C0;
    ParticleSprayVector x26D0;
    char pad26E0[0x20];
    ParticleSprayVector x2700;
    char pad2710[0x14];
    short x2724;
    short x2726;
    char pad2728[0x4];
    float x272C;
    float x2730;
    char pad2734[0x8];
    float x273C;
    float x2740;
    float x2744;
    float x2748;
    float x274C;
    char pad2750[0x1C];
    unsigned short x276C;
    unsigned short x276E;
    char pad2770[0x6A];
    short x27DA;
    char pad27DC[0x40];
    int x281C;
    char pad2820[0x8];
    int x2828;
    int x282C;
    ParticleSprayVector *x2830;
    ParticleSprayVector *x2834;
    short *x2838;
    int *x283C;
    char pad2840[0x8];
    void *x2848;
    char pad284C[0x4];
    int x2850;
    int x2854;
    char pad2858[0x2];
    short x285A;
    signed char x285C;
    signed char x285D;
    signed char x285E;
} ParticleSprayEmitter;
typedef char ParticleSprayEmitter_size_check[sizeof(ParticleSprayEmitter) == 0x2860 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void func_0023F650(ParticleSprayEmitter *, ParticleSprayVector *);
void func_0023FAE8(ParticleSprayEmitter *, ParticleSprayVector *);
#if defined(__mips__)
void EmitParticleSprayLegacy(void *, void *, int) __asm__("func_0023F650");
void EmitParticleSprayVariantLegacy(void *, void *, int) __asm__("func_0023FAE8");
#else
static inline void EmitParticleSprayLegacy(void *emitter, void *position, int unused) { func_0023F650((ParticleSprayEmitter *)emitter,(ParticleSprayVector *)position); }
static inline void EmitParticleSprayVariantLegacy(void *emitter, void *position, int unused) { func_0023FAE8((ParticleSprayEmitter *)emitter,(ParticleSprayVector *)position); }
#endif
#ifdef __cplusplus
}
#endif
#endif
