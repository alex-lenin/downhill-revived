/* UNIT 0x00182D78 0x001831A0 .rodata=0x28C590:0x28C5A0 .sbss=0x29DDD8:0x29DDDC */
#include "render_wait.h"
typedef struct { int x0, x4, x8, xc; } Sound;

typedef struct {
    char pad0[0x10];
    int x10;
    float x14;
    float x18;
} Cam;

typedef struct {
    char pad0[0x8];
    unsigned short x8;
    char padA[0x20 - 0xA];
    int *x20;
    int *x24;
    int *x28;
    int *x2C;
} Model;

typedef struct {
    Model *x0;
    unsigned char x4;
    char pad5[0x10 - 0x5];
    int x10;
    int x14;
    int x18;
} Bike;

typedef struct {
    char pad0[0x8];
    short x8;
} Rider;

typedef struct {
    int x0;
    Rider *x4;
    Bike *x8;
    char padC[0x90 - 0xC];
    Sound x90;
    int xA0;
    int xA4;
} Shop;

typedef struct { char pad[0x18]; } Entry;

typedef struct {
    int x0;
    int x4;
    int x8;
} Snd;

extern void func_00238B20(void);
extern void func_00238BD8(void);
extern int func_00243CE8(int);
extern void func_0023D6C8(void);
extern void func_0023D610(int);
extern void func_0023C8D8(int, int);
extern Cam *func_0023D040(int);
extern Bike *func_00208FB8(void);
extern void func_00218278(Bike *, int);
extern void func_00203590(void *, int, int, int);
extern void func_002038E0(void *, int);
extern int func_0023D5F0(int);
extern void func_0025A960(int);
extern void func_0021F0A0(int, int, int);
extern void func_0021F1B8(int, int, int, int);
extern int func_0021F3F8(void);
extern void func_0021F210(int, int, int, int, int);
extern void func_0023CB00(int);
extern void func_00218720(Bike *, int, int, int);
extern void func_002065C0(void);
extern void func_002102A8(void *, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F9A8(int, int, int, int, char *);
extern void func_0020F2A8(int);
extern void func_0014DE90(Entry *);

extern void func_00210460(void *, int, int);
extern void func_002433A8(int, int);
extern void func_00238B40(void);
extern void func_00238C00(int *);
extern void func_00238D98(int *);
extern void func_002215A8(void *);
extern void func_00221310(int, int);
extern void func_00221578(Bike *);
extern void func_00221438(void);
extern int func_0025A3C0(int, void (*)(void), int);
extern void func_0025AF98(int);

extern char D_00513B00[];
extern char D_004B1F40[];
extern Entry D_00381CF0[];
extern Snd D_003C6360;

static int D_0029DDD8 __attribute__((section(".sbss"))) = 0;

void func_00182D78(Shop *shop, int mode)
{
    Sound snd;
    int flag;
    int i;
    int n;
    int on;
    Cam *cam;
    Model *model;

    i = 0;
    on = 1;
    func_00238B20();
    func_00238BD8();
    flag = func_00243CE8(0) == 0;
    func_0023D6C8();
    func_0023D610(1);
    func_0023C8D8(0, 0);
    func_0023C8D8(11, 5);
    cam = func_0023D040(5);
    cam->x10 = 0;
    cam->x14 = -73.13f;
    cam->x18 = -4.45f;
    shop->x8 = func_00208FB8();
    shop->x8->x4 = on;
    func_00218278(shop->x8, shop->x0);
    shop->x8->x10 = 0;
    shop->x8->x14 = 0;
    shop->x8->x18 = 0;
    if (shop->x4 != 0 && shop->xA4 >= 0 && shop->xA4 >= 9) {
        shop->x4->x8 = 0;
    }
    func_00203590(&shop->x90, 0xC1D, 0, 0);
    snd = shop->x90;
    func_002038E0(&snd, 1);
    for (n = 0; n < 8; n++) {
        int obj;

        obj = func_0023D5F0(5);
        func_0025A960(0);
        if (i < 4) {
            func_0021F0A0(obj, (i ^ 1) & 1, 1);
        } else {
            func_0021F1B8(obj, i & 1, 0, 1);
        }
        if (func_0021F3F8()) {
            func_0021F210(5, obj, i, flag, 1);
        } else {
            func_0021F210(5, obj, i, 0, 1);
        }
        func_0023CB00(0);
        func_0023CB00(5);
        func_00218720(shop->x8, (i ^ 1) & 1, flag, 5);
        func_002065C0();
        func_002102A8(D_00513B00, 1, (i ^ 1) & 1);
        if (mode == 99) {
            func_0020FB80(0, 0x140, 0x120, -0x75, -0x20);
            func_0020F518(0, 0x6E, 0x6E, 0xC8, 0x96);
            func_0020F9A8(0, 0, 10, 0x96, "Bike Shop");
            func_0020F2A8(0);
        } else if (shop->xA4 >= 0 && mode != 0) {
            func_0014DE90(&D_00381CF0[shop->xA4]);
        }
        i++;
        WaitForRenderLegacy(D_004B1F40);
        func_00210460(D_00513B00, 0, 1);
        func_002433A8(0, 0);
        func_00238B40();
        func_00238C00(&flag);
        func_00238D98(&flag);
    }
    model = shop->x8->x0;
    if (model->x8 == 3) {
        *model->x20 = (*model->x20 & ~0x3F) | 9;
        *model->x24 = (*model->x24 & ~0x3F) | 9;
    } else if (model->x8 == 4) {
        *model->x24 = (*model->x24 & ~0x3F) | 9;
        *model->x28 = (*model->x28 & ~0x3F) | 9;
        *model->x2C = (*model->x2C & ~0x3F) | 9;
    }
    if (!D_0029DDD8) {
        D_003C6360.x8 = 0;
        D_0029DDD8 = 1;
    }
    func_00203590(&D_003C6360, 0xC1D, 0, 0);
    func_002215A8(&D_003C6360);
    func_00221310(flag, i);
    func_00221578(shop->x8);
    shop->xA0 = func_0025A3C0(2, func_00221438, 0);
    func_0025AF98(2);
}
