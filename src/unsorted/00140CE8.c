/* UNIT 0x00140CE8 0x001410B8 */
#include "disc_path.h"
typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct Heap { int x0; char *x4; } Heap;
typedef struct Loader { char pad0[0x20]; int state; char pad24[4]; int x28; } Loader;
typedef struct Flag { char pad0[4]; char x4; } Flag;
typedef struct Obj {
    Vec3 x0, xC;
    Vec3 x18, x24;
    Heap x30;
    char pad38[0x10];
    char x48[0x18];
    Loader x60;
    char pad8C[4];
    unsigned char x90, x91;
    char pad92[0x12];
    int xA4;
    char xA8[0x60];
    unsigned char x108;
    char pad109[3];
    int x10C[6];
    int x124;
    int x128;
    unsigned char x12C;
    char pad12D[3];
    int x130;
} Obj;

extern char D_0029C538[]; /* "BIKE" */
extern char D_0029C540[]; /* "R" */
extern int D_0026E460[];
extern int D_0026E470[];
extern char D_004D30B0[];
extern char D_004D1A18[];
extern char D_004D4748[];
extern Flag *D_0029C468;
extern Flag *D_0029C46C;
extern Flag *D_0029DD34;

extern int func_00207FA0(Loader *);
extern int func_00208008(Loader *, void *, char *, void *, int);

extern void func_002421C0(int);
extern int func_00242118(int, int);
extern void func_00223B00(void *);
extern void func_0020D5B8(int, int, int, int, void *, int);
extern void func_00219438(void);
extern int func_0020D170(char *, int);
extern int func_002195F0(int, int, int);
extern void func_00219488(int, int);
extern void func_0020B228(int, int, int);
extern void func_001418F8(Obj *, Vec3 *, Vec3 *);
extern int func_0013AF60(int);
extern void func_00218498(int, int);

int func_00140CE8(Obj *obj) {
    char buf[0x40];
    Loader *ld;
    int loaded;
    Heap *heap;
    Loader *ld2;
    int index;
    int part;
    int base;
    int size;
    int i;

    ld = &obj->x60;
    loaded = 0;
    index = obj->x128 / obj->xA4;
    part = obj->x128 % obj->xA4;
    if (ld->state == 7) {
        func_00207FA0(ld);
    } else if (ld->state == 6) {
        func_00207FA0(ld);
    } else if (obj->x90) {
        obj->x90 = 0;
        obj->x12C = 0;
        return 0;
    } else if (!obj->x108) {
        func_00208008(ld, &obj->xA8[part * 16], part ? D_0029C538 : D_0029C540, obj->x48, D_0026E460[index]);
        obj->x108 = 1;
    } else if (func_00207FA0(&obj->x60) == 5) {
        BuildDiscPathWithMode((char *)(buf), (const char *)(part ? D_0029C538 : D_0029C540), (const char *)(&obj->xA8[part * 16]), (const char *)(D_0026E460[index]), 1);
        heap = &obj->x30;
        base = ((int)heap->x4 + 0x7F) & ~0x7F;
        heap->x4 = (char *)base + 1;
        ld2 = &obj->x60;
        loaded = 1;
        func_002421C0(ld2->x28);
        size = func_00242118(base, ld2->x28);
        heap->x4 += size - 1;
        func_00223B00(obj->x48);
        func_0020D5B8(D_0026E470[index], base, size, obj->x130 + part, buf, 0);
    }
    if (!loaded) {
        return 1;
    }
    obj->x108 = 0;
    obj->x128++;
    if (obj->x128 <= obj->xA4) {
        if (obj->x128 == obj->xA4) {
            obj->x124 = 0x3F80;
        }
    } else {
    if (obj->x128 != obj->xA4 * 4) {
        return 1;
    }
    func_00219438();
    for (i = 0; i < obj->xA4; i++) {
        obj->x124 = obj->x10C[i] = func_002195F0(func_0020D170(D_004D30B0, obj->x130 + i), obj->x130 + i, obj->x124);
    }
    for (i = 0; i < obj->xA4; i++) {
        func_00219488(func_0020D170(D_004D1A18, obj->x130 + i), obj->x130 + i);
    }
    obj->x124 = 0x3F80;
    for (i = 0; i < obj->xA4; i++) {
        func_0020B228(func_0020D170(D_004D4748, obj->x130 + i), obj->x124, obj->x130 + i);
        obj->x124 = obj->x10C[i];
    }
    func_001418F8(obj, &obj->x18, &obj->x24);
    obj->x0 = obj->x18;
    obj->xC = obj->x24;
    obj->x91 = 1;
    if (D_0029C468) D_0029C468->x4 = 1;
    if (D_0029C46C) D_0029C46C->x4 = 1;
    if (D_0029DD34) D_0029DD34->x4 = 1;
    func_00218498(func_0013AF60(0x2598), 1);
    obj->x12C = 0;
    return 0;
    }
    return 1;
}
