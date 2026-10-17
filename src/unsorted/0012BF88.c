/* UNIT 0x0012BF88 0x0012C110 */
#include "scene.h"
#include "animation.h"
typedef AnimationHandle Handle;

typedef struct VTab {
    char pad0[0x10];
    short x10;
    short pad12;
    void (*x14)(void *);
} VTab;

typedef struct Item {
    int x0;
    VTab *x4;
} Item;

typedef SceneNode Body;

typedef struct Part {
    char pad0[0x16D4];
    Body *x16D4;
    char tail[0xA8C];
} Part;

typedef struct Obj {
    Handle x0[0xFE];
    char xFE0[0xE60];
    Part x1E40;
    char pad3FA4[0xF28];
    void *x4ECC;
    char pad4ED0[0x1110];
    char x5FE0[0x3B4];
    Item *x6394[18];
    char x63DC[0x44];
    char x6420[0x1F0];
    char x6610[0x12C8];
    char x78D8[0x54];
    Body *x792C;
    char pad7930[0x40];
    int x7970;
    char pad7974[8];
    unsigned short x797C;
    char pad797E[0x176];
    int x7AF4;
} Obj;

typedef struct Glob {
    char pad0[0x2A0];
    int x2A0;
} Glob;

extern Glob D_004B25A0;
extern void func_001AC150(void *);
 /* 5th arg travels in $t0 */
extern void func_0015BFC0(void *);
extern void func_0018D2F8(void *);
extern void func_001FA858(void *); /* 1 arg; $a1 still holds x792C at the call */
extern void func_001E5680(void *);
extern void func_00133940(void *);
extern void func_0012DE30(void *);
extern void func_0017D720(void *, void *);
extern void func_001E61D0(void *);

void func_0012BF88(Obj *obj)
{
    int i;
    Item **pp;
    Item *cur;
    VTab *v;
    int n;
    Part *part;
    int p7970;
    char *p63DC;
    char *p5FE0;
    char *p78D8;
    char *p6610;
    char *p6420;

    func_001AC150(&obj->x1E40);
    p7970 = obj->x7970;
    for (i = 1; i < 0xFE; i++)
        func_002036C8(&obj->x0[i], p7970, 0, i, obj->x7AF4);
    part = &obj->x1E40;
    p63DC = &obj->x63DC[0];
    p5FE0 = &obj->x5FE0[0];
    p78D8 = &obj->x78D8[0];
    p6610 = &obj->x6610[0];
    p6420 = &obj->x6420[0];
    pp = &obj->x6394[0];
    n = 0x11;
    do {
        cur = *pp;
        n--;
        pp++;
        v = cur->x4;
        v->x14((char *)cur + v->x10);
    } while (n >= 0);
    func_0015BFC0(p63DC);
    obj->x4ECC = p5FE0;
    func_0018D2F8(p5FE0);
    part->x16D4->flags |= (SceneNodeFlags)1 << 62;
    part->x16D4->xB6 = obj->x797C;
    ((Body *)obj->x792C)->xB6 = obj->x797C;
    func_001FA858(p78D8);
    func_001E5680(p6610);
    func_00133940(p6420);
    func_0012DE30(obj);
    func_0017D720(&obj->xFE0, obj);
    if (D_004B25A0.x2A0 == 0x1B)
        func_001E61D0(p6610);
}
