/* UNIT 0x00147958 0x00147A30 */
#include "particle_descriptor.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct State {
    unsigned int id : 6;
} State;

typedef struct Model {
    char pad0[0xC];
    float xC;
    char pad10[0x60];
    Vec4 x70;
} Model;

typedef struct Actor {
    char pad0[0x8];
    State *state;
    Model *model;
} Actor;

typedef struct World {
    char pad0[0x4180];
    Vec4 x4180;
    char pad4190[0x7720 - 0x4190];
    char x7720[1];
} World;


extern void func_001C81B0(void *);

static inline float InnerProduct(Vec4 *a, Vec4 *b) {
    float result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(result) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return result;
}

void func_00147958(Actor *actor, World *world) {
    if (actor->model != 0 && actor->state->id == 6) {
        Vec4 zero = { 0 };
        actor->model->xC = InnerProduct(&world->x4180, &actor->model->x70) * 0.02f + 0.25f;
        SpawnParticleDescriptionDiscard((char *)actor->model + 0xA0, (char *)actor->model + 0x60, actor->model, &zero);
        actor->state->id = 9;
        func_001C81B0(world->x7720);
    }
}
