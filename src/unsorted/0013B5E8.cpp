/* UNIT 0x0013B5E8 0x0013B9C0 */
#include "menu.h"
#include "animation.h"
/* C++: func_0013B5E8 clamps with g++'s <? and >? (min.s/max.s that reorg puts in delay slots) */
__asm__(".extern D_0029C4B4, 4");
__asm__(".extern D_0029DD0C, 4");
extern "C" {

typedef struct { int x0; short x4; char x6, x7; int x8, xC; } Sub830;
typedef struct { char pad[0x4F8]; void *x4F8; int x4FC, x500; Sub830 sub; int x514, x518, x51C, x520; } Obj830;

typedef AnimationHandle Obj361680;

struct Player {
    char pad0[0x524];
    int x524;
};

extern int D_0029DD0C;
extern char D_00284190[];
extern unsigned int *D_0029E4A4;
extern unsigned char D_003F52D0[];
extern Obj361680 D_00361680;

extern int func_00238F48(void);

extern void func_001B5AF0(void *, int, int);
extern void func_00218498(unsigned int *, int);

/* one color channel (or the alpha) of a sound/light control: its id, then its level */
typedef AnimationHandle Control;
typedef struct ColorIds { int id[3]; } ColorIds;
extern ColorIds D_00289A78;
extern int D_0029C4B4;
extern float func_001FA670(void *);
extern float func_001FA6F0(void *);
extern float func_001FA758(void *);


/* Moves the three color channels and the alpha toward the entry's color (or toward
   black and transparent without one), at most 0.05 per frame, once per frame. */
void func_0013B5E8(void *entry, int red, int green, int blue)
{
    float color[3];
    ColorIds ids;
    Control ctrl;
    float alpha, level;
    int i, j;

    if (entry == 0 && func_00238F48() == D_0029C4B4)
        return;
    D_0029C4B4 = func_00238F48();
    for (j = 2; j >= 0; j--)
        color[j] = 0.0f;
    alpha = 0.0f;
    if (entry != 0) {
        color[0] = func_001FA670(entry) * 0.8f + 0.2f;
        color[1] = func_001FA6F0(entry) * 0.8f + 0.2f;
        color[2] = func_001FA758(entry) * 0.8f + 0.2f;
        alpha = 1.0f;
    }
    ids = D_00289A78;
    if (red) ids.id[0] = red;
    if (green) ids.id[1] = green;
    if (blue) ids.id[2] = blue;
    for (i = 0; i < 3; i++) {
        float t;
        ctrl.x8 = 0;
        func_00203590(&ctrl, ids.id[i], 0, 0);
        func_002038E0(&ctrl, 0);
        func_00203E10(&ctrl, 0.0f);
        t = func_00203C28(&ctrl);
        func_00203B68(&ctrl, 0, (color[i] <? t + 0.05f) >? t - 0.05f);
    }
    ctrl.x8 = 0;
    func_00203590(&ctrl, 0x2589, 0, 1);
    func_002038E0(&ctrl, 0);
    func_00203E10(&ctrl, 0.0f);
    level = func_00203C28(&ctrl);
    func_00203B68(&ctrl, 0, (alpha <? level + 0.05f) >? level - 0.05f);
}


Obj830 *func_0013B830(Obj830 *obj) {
    Sub830 *sub;
    int i;
    int *p;
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00284190;
    sub = &obj->sub;
    sub->x4 = 0;
    sub->x7 = 0;
    sub->x8 = 0;
    sub->xC = 0;
    obj->x500 = 0;
    p = &obj->x51C;
    i = 2;
    do {
        *p = -1;
        i--;
        p--;
    } while (i >= 0);
    obj->x520 = -1;
    return obj;
}

void func_0013B8B0(unsigned int *p)
{
    if ((*p >> 18) == 0x2584) {
        D_0029E4A4 = p;
    }
}

void func_0013B8D0(struct Player *player)
{
    func_001B5AF0(D_003F52D0, 2, 1);
    func_001B5AF0(D_003F52D0, 0, 1);
    func_001B5AF0(D_003F52D0, 1, 0);
    func_001B5AF0(D_003F52D0, 7, 1);
    if (player->x524 == 0) {
        if (!D_0029DD0C) {
            D_00361680.x8 = 0;
            D_0029DD0C = 1;
        }
        func_00203590(&D_00361680, 0x2588, 0, 1);
        func_00204080(&D_00361680, 1);
        func_00202E38(&D_00361680, 1, 7, 200.0f);
    }
    if (D_0029E4A4 != 0) {
        func_00218498(D_0029E4A4, 2);
    }
}

}
