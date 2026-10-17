/* UNIT 0x001B4AF8 0x001B4CD0 */
#include "render_wait.h"
typedef struct Entry {
    void *x0;
    void *x4;
    int x8;
    int xC;
    int x10;
} Entry;

typedef struct Rider {
    char pad0[0x127C];
    int x127C;
    char pad1280[0x1380 - 0x1280];
} Rider;

typedef struct Riders {
    char pad0[0x150];
    Rider riders[4];
} Riders;

typedef struct Obj {
    char pad0[0x3];
    unsigned char x3;
    unsigned char x4;
    char pad5[0xC - 0x5];
    int xC;
    int x10[4];
    int x20;
    Entry x24;
    int x38;
    int x3C;
    char pad40[0x56 - 0x40];
    unsigned char x56;
} Obj;

extern Entry D_0075A9D0[];
extern Riders D_003B2270;
extern char D_004B1F40[], D_004B25A0[];
extern int D_003F5400[];
extern int func_0023D608(void);
extern void func_0023D610(int);
extern void func_0023C8D8(int, int);
extern void func_0023D020(int, int);
extern int func_0023D040(int);

extern void func_001B8CD8(void *, int);
extern void func_00176E40(void *, int);

static inline void SetSlot(int index, int value)
{
    if (index >= 0 && index < 4) {
        func_00176E40(&D_003B2270.riders[index], value);
    }
}

void func_001B4AF8(Obj *obj)
{
    int count;
    int i;

    obj->xC = 0;
    obj->x20 = 0;
    obj->x56 = 1;
    count = func_0023D608();
    if (obj->x3 || obj->x4) {
        WaitForRenderLegacy(D_004B1F40);
        func_001B8CD8(D_004B25A0, 0);
        if (obj->x4) {
            obj->x38 = D_0075A9D0[obj->x20].x8;
            func_0023D610(1);
            func_0023C8D8(5, 0);
            goto slots;
        }
    }
    if (count >= 2) {
        obj->xC = func_0023D040(obj->x20);
        func_0023D020(obj->x20, func_0023D040(D_003F5400[0]));
        obj->x24 = D_0075A9D0[obj->x20];
        obj->x38 = D_0075A9D0[obj->x20].x8;
        func_0023C8D8(0, obj->x20);
    } else {
        obj->x20 = 0;
        obj->x38 = 0;
    }
slots:
    for (i = 0; i < count; i++) {
        obj->x10[i] = D_003B2270.riders[i].x127C;
        SetSlot(i, 0);
    }
    obj->x3C = func_0023D608();
    func_0023D610(1);
}
