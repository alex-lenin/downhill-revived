#ifndef DHD_PARTICLE_DESCRIPTOR_H
#define DHD_PARTICLE_DESCRIPTOR_H
typedef unsigned int ParticleDescriptorQuad __attribute__((mode(TI)));
typedef unsigned long ParticleDescriptorFlags __attribute__((mode(DI)));
typedef union ParticleDescriptorState {
    ParticleDescriptorFlags l, all;
    float f;
    struct { ParticleDescriptorFlags pad0:32, fixed:1, pad33:3, active:1, pad37:27; } bits;
} ParticleDescriptorState;
typedef struct ParticleEmitterDescription {
    int x0, type; float x8, xC; ParticleDescriptorQuad x10;
    float x20, x24, x28, x2C; int x30, x34; float x38; unsigned int x3C;
    float x40, x44; int x48; float x4C, x50; int x54;
    ParticleDescriptorState x58;
    float x60[16] __attribute__((aligned(16)));
    float xA0[4] __attribute__((aligned(16)));
} ParticleEmitterDescription;
typedef char ParticleDescriptorState_width_check[sizeof(((ParticleDescriptorState *)0)->l) == 8 ? 1 : -1];
typedef char ParticleDescriptorMatrix_size_check[sizeof(((ParticleEmitterDescription *)0)->x60) == 64 ? 1 : -1];
typedef char ParticleEmitterDescription_size_check[sizeof(ParticleEmitterDescription) == 0xB0 ? 1 : -1];
#if !defined(__mips__)
struct Vector4; struct Matrix4;
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__mips__)
int SpawnParticleDescription(void *, void *, void *, void *) __asm__("func_0022D1B0");
void SpawnParticleDescriptionDiscard(void *, void *, void *, void *) __asm__("func_0022D1B0");
#else
int func_0022D1B0(struct Vector4 *, struct Matrix4 *, ParticleEmitterDescription *, struct Vector4 *);
static inline int SpawnParticleDescription(void *pos, void *matrix, void *description, void *offset) { return func_0022D1B0((struct Vector4 *)pos,(struct Matrix4 *)matrix,(ParticleEmitterDescription *)description,(struct Vector4 *)offset); }
static inline int SpawnParticleDescriptionDiscard(void *pos, void *matrix, void *description, void *offset) { return SpawnParticleDescription(pos,matrix,description,offset); }
#endif
#ifdef __cplusplus
}
#endif
#endif
