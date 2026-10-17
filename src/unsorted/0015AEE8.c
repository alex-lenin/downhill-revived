/* UNIT 0x0015AEE8 0x0015B018 */
#include "particle_emission.h"
#if defined(__mips__)
#define CallShared00162690_0 func_00162690
#else
#define CallShared00162690_0 Forward00162690_2
#endif
#include "rider_geometry.h"
typedef struct Vec4 {
    float v[4];
} Vec4;

typedef struct Item {
    char pad0[0x4];
    int state;
    int x8;
    char padC[0x98 - 0xC];
    Vec4 x98;
    char padA8[0xF0 - 0xA8];
    char xF0[0x140 - 0xF0];
    char x140[0x10];
    char x150[0x180 - 0x150];
    char x180[0x1E2 - 0x180];
    signed char x1E2;
    char pad1E3;
    short x1E4;
    char pad1E6[2];
    short x1E8;
} Item;

extern void func_0015B018(Item *item);
extern void func_0015B380(Item *item);
extern void func_0015B460(Item *item);
extern void func_0015B538(Item *item);
extern void func_0015B798(Item *item);
extern void func_0015A890(Item *item);
extern void func_00159CE0(Item *item, int state);
extern void func_00162690(void *p, float f, int a, int b);

extern float func_00238F40(void);
extern void func_00236558(void *a, void *b, float f);
extern float func_00203C28(Vec4 *v);

void func_0015AEE8(Item *item)
{
    char buf[16];
    Vec4 v;

    if (item->x1E8 != 0) {
        func_0015B018(item);
        return;
    }
    if (item->x1E4 == 0) {
        func_0015B380(item);
    }
    if (item->x1E2 != 0) {
        func_0015B460(item);
        CallShared00162690_0(item->x180, 1.0f, 1, item->x8);
    }
    if (item->x1E4 != 0) {
        func_0015B538(item);
    }
    func_00260E08(item->xF0, item->x140, item->x150, buf);
    func_00236558(item->x140, buf, func_00238F40());
    func_0015B798(item);
    func_0015A890(item);
    if (item->state == 7) {
        v = item->x98;
        if (func_00203C28(&v) >= 1.0f) {
            func_00159CE0(item, 6);
        }
    }
}
