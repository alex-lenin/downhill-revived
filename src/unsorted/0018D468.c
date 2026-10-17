/* UNIT 0x0018D468 0x0018D5A0 */
#include "camera.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Node {
    char pad0[0x10];
    Vec x10;
} Node;

typedef struct Control {
    char pad0[0xC8];
    unsigned long xC8;
} Control;

typedef struct World {
    char pad0[0xFE0];
    char xFE0[0x3530 - 0xFE0];
    char x3530[0x3FD0 - 0x3530];
    Control x3FD0;
    char pad[0x43D0 - 0x3FD0 - sizeof(Control)];
    unsigned long x43D0;
    char pad43D8[0x4450 - 0x43D8];
    unsigned long x4450;
    char pad4458[0x6300 - 0x4458];
    int x6300;
    char pad6304[0x792C - 0x6304];
    Node *x792C;
    char pad7930[0x797C - 0x7930];
    int x797C;
    char pad7980[0x7984 - 0x7980];
    int x7984;
    int x7988;
    char pad798C[0x7AB0 - 0x798C];
    Vec x7AB0;
} World;

typedef struct Obj {
    World *world;
} Obj;

typedef struct Player {
    char pad0[0x2B8];
    int x2B8;
    char pad2BC[0x380 - 0x2BC];
} Player;

extern Player D_0037EA60[];

extern void func_00156A70(Control *control);
extern void func_0012DDC0(World *world);
extern void func_00162D98(int value, int index);
extern void func_0017D740(void *p, World *world);


extern void func_0012D660(World *world, int index, float scale);
extern int func_00148B68(Player *players, World *world);
extern void func_0020CD78(int sound, float a, float b);

/* 128-bit vector copy (libvu0-style inline asm) */
static inline void VecCopy(Vec *out, Vec *in)
{
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*out) : "r"(in) : "$2");
}

static inline int IsLocked(Control *control)
{
    return (unsigned char)(control->xC8 >> 2) & 1;
}

static inline int GetState(Player *players, int index)
{
    return players[index].x2B8;
}

void func_0018D468(Obj *obj)
{
    World *world = obj->world;
    float volume;

    if (!IsLocked(&world->x3FD0)) {
        func_00156A70(&world->x3FD0);
        func_0012DDC0(world);
        VecCopy(&world->x7AB0, &world->x792C->x10);
        world->x43D0 &= ~(0x1FUL << 48);
        world->x4450 &= ~(0x1FUL << 48);
        func_00162D98(0, world->x797C);
        func_0017D740(world->xFE0, world);
        func_001B33A8(world->x3530);
        func_001B0E00((RiderCamera *)(world->x3530));
        func_0012D660(world, -1, 1.0f);
        if (world->x7984 != 0 && GetState(D_0037EA60, world->x7988) != 4) {
            volume = 0.125f;
            func_0020CD78(func_00148B68(D_0037EA60, world), volume, volume);
        }
    }
    world->x6300 = 60;
}
