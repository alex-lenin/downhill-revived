/* UNIT 0x001DCB50 0x001DCCA8 */
#include "particle.h"
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Item {
    char pad0[0x10];
    Vec pos;
} Item;

typedef struct Obj {
    Vec pos;
    char pad10[0x30 - 0x10];
    Item *x30;
    char pad34[0x40 - 0x34];
    float x40;
    char pad44[0x48 - 0x44];
    int x48;
    int x4C;
    char pad50[0x54 - 0x50];
    int x54;
} Obj;

typedef struct Game {
    char pad0[0x240];
    char x240[0x5BF8 - 0x240];
    int *x5BF8;
} Game;

extern Game D_00398C40;
extern int func_00238F60(void);
extern void func_001DCD38(Obj *obj);
extern void func_001DCB48(Obj *obj);
extern void func_001D45E8(void *a, Vec *b);

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vadd.xyz $vf11, $vf11, $vf12\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

void func_001DCB50(Obj *obj)
{
    Game *game;
    Vec *pos;
    float volume;

    obj->x48 += func_00238F60();
    switch (obj->x54) {
    case 1:
        if (obj->x48 >= 60) {
            obj->x54 = 2;
        }
        break;
    case 2:
        func_001DCD38(obj);
        if (obj->x40 < obj->x30->pos.z && obj->x4C < 16 && obj->x48 < 901) {
            obj->pos.z -= 0.03f;
            VecAdd(&obj->x30->pos, &obj->x30->pos, &obj->pos);
        } else {
            func_001DCB48(obj);
            game = &D_00398C40;
            obj->x54 = 3;
            func_001D45E8(game->x240, &obj->x30->pos);
            pos = &obj->x30->pos;
            volume = 0.25f;
            if (*game->x5BF8 == 5) {
                volume = 0.5f;
            }
            func_0022CDC0((Vector4 *)(pos), 0x45, (Matrix4 *)(0), (int)(0), volume, (Vector4 *)(0));
        }
        break;
    }
}
