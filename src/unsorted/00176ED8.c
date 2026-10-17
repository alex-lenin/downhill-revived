/* UNIT 0x00176ED8 0x001772B0 */
#include "rider_geometry.h"
typedef struct Obj Obj;
typedef struct Rider Rider;
typedef struct Record Record;

struct Record {
    char pad0[0x10];
    volatile unsigned char x10;
    char pad11[0x3];
    unsigned short flags;
};

struct Rider {
    char pad0[0x7954];
    Record *record;
    char pad7958[0x4];
    volatile float x795C;
};

struct Obj {
    char pad0[0x1164];
    Rider *rider;
    char pad1168[0x114];
    int x127C;
    int player;
    char pad1284[0x8C];
    int x1310;
    char pad1314[0x10];
    int x1324;
    char pad1328[0x39];
    unsigned char x1361;
    unsigned char x1362;
    char pad1363[0xD];
    int x1370;
};

typedef struct {
    char pad0[0x10C];
    int x10C;
    char pad110[0x178];
    int x288;
} GlobalB25A0;

typedef struct {
    char pad0[0x488];
    int x488;
} Global3EEC0;

extern GlobalB25A0 D_004B25A0;
extern Global3EEC0 D_0043EEC0;
extern char D_0029C960[];
extern char D_0028BBD8[];

extern void func_001772B0(Obj *);
extern void func_0017C638(Obj *);
extern void func_001778D0(Obj *);
extern void func_00177AC8(Obj *);
extern void func_00178730(Obj *);
extern void func_00179140(Obj *);
extern void func_00179A60(Obj *);
extern void func_0017B0D8(Obj *);

extern void func_001778E8(Obj *);
extern void func_00179338(Obj *);
extern void func_0017A678(Obj *, int);
extern void func_00179630(Obj *);
extern void func_0017B3C0(Obj *);
extern void func_00179E80(Obj *);
extern void func_0017A0E0(Obj *);
extern void func_00179FA8(Obj *);
extern void func_0017BA30(Obj *);
extern void func_00177650(Obj *);
extern void func_00177770(Obj *);
extern void func_00177628(Obj *);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, char *);

void func_00176ED8(Obj *obj)
{
    Rider *rider;
    Record *record;
    int show;
    int mode;
    float fv;
    float fm;

    if (D_004B25A0.x10C == 15) {
        func_001772B0(obj);
    } else if (D_004B25A0.x288 < 3) {
        if (obj->x127C != 0) {
            if (D_004B25A0.x10C == 10) {
                --obj->x1324;
                if (obj->x1324 < 121) {
                    mode = D_0043EEC0.x488;
                    if ((unsigned)(mode - 2) < 3) {
                        goto draw_timer_1;
                    }
                    if (mode != 1) {
                        goto after_draw_timer_1;
                    }
draw_timer_1:
                    func_0020F588(obj->player, 0, 120, 80, 20, 128);
                    func_0020FB80(0, 700, 500, -120, 0);
                    func_0020F9F0(0, obj->player, 256, 20, D_0029C960);
                }
after_draw_timer_1:
                mode = D_0043EEC0.x488;
                if ((unsigned)(mode - 2) < 3) {
                    goto draw_timer_2;
                }
                if (mode != 1) {
                    goto after_draw_timer_2;
                }
draw_timer_2:
                {
                    func_0020F588(obj->player, 0, 110, 110, 200, 150);
                    func_0020FB80(0, 256, 192, -91, -32);
                    func_0020F9F0(0, obj->player, 256, 200, D_0028BBD8);
                }
after_draw_timer_2:
                if (obj->x1324 <= 0) {
                    obj->x1324 = 240;
                }
            } else {
                if (obj->x1310 > 0) {
                    func_0017C638(obj);
                    --obj->x1310;
                }
                func_001778D0(obj);
                mode = D_0043EEC0.x488;
                if ((unsigned)(mode - 2) < 3) {
                    goto normal_updates;
                }
                if (mode != 1) {
                    goto after_normal_updates;
                }
normal_updates:
                {
                    func_00177AC8(obj);
                    func_00178730(obj);
                    func_00179140(obj);
                    func_00179A60(obj);
                    rider = obj->rider;
                    record = rider->record;
                    show = 0;
                    if (record == 0) {
                        goto record_checked;
                    }
                    if (((record->flags >> 1) & 1) == 0) {
                        goto record_checked;
                    }
                    fv = rider->x795C;
                    fm = __builtin_fabsf(fv);
                    /* Zero-byte label: scheduling barrier so the byte load stays
                       after abs.s, input keeps fv live so load/abs use $f0/$f1. */
                    __asm__ volatile ("Lshow_lim:" : : "f" (fv));
                    if (fm < (float)(record->x10 * 3)) {
                        show = 1;
                    }
record_checked:
                    if ((show & 0xFF) != 0) {
                        func_0017B0D8(obj);
                    }
                    if (D_004B25A0.x288 == 2) {
                        func_001797A0((RiderMarkerHud *)(obj));
                    }
                    if (obj->x1370 > 0 && D_004B25A0.x288 == 1) {
                        func_001778E8(obj);
                    } else {
                        obj->x1362 = 0;
                        obj->x1361 = 1;
                    }
                    func_00179338(obj);
                }
after_normal_updates:
                func_0017A678(obj, 1);
                func_00179630(obj);
                func_0017B3C0(obj);
                if (D_004B25A0.x10C == 18 &&
                    (unsigned)(D_0043EEC0.x488 - 2) < 3) {
                    func_00179E80(obj);
                } else if (D_004B25A0.x10C == 5 &&
                           (unsigned)(D_0043EEC0.x488 - 2) < 3) {
                    func_0017A0E0(obj);
                } else if (D_004B25A0.x10C == 16 &&
                           (unsigned)(D_0043EEC0.x488 - 2) < 3) {
                    func_00179FA8(obj);
                }
                func_0017BA30(obj);
            }
        }
    } else if (obj->x127C != 0) {
        func_001778D0(obj);
        func_00177650(obj);
        func_00177770(obj);
        func_00178730(obj);
        func_00177628(obj);
        func_001797A0((RiderMarkerHud *)(obj));
        func_0017B3C0(obj);
        func_0017BA30(obj);
    }
}
