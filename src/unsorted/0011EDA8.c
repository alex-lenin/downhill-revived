/* UNIT 0x0011EDA8 0x0011F1E0 .rodata=0x288CD0:0x288D00 */
#include "rider_queries.h"
#if defined(__mips__)
#define CallShared0011C0D0_0 func_0011C0D0
#else
#define CallShared0011C0D0_0 Forward0011C0D0_0
#endif
#if defined(__mips__)
#define CallShared0011C168_1 func_0011C168
#else
#define CallShared0011C168_1 Forward0011C168_1
#endif
#if defined(__mips__)
#define CallShared0011C168_2 func_0011C168
#else
#define CallShared0011C168_2 Forward0011C168_1
#endif
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Model {
    char pad0[0x10];
    Vec4 pos;
} Model;

typedef struct Handle { int x0; void *x4; int x8; int xC; } Handle;

typedef struct Obj {
    char pad0[0x4];
    int type;
    int state;
    char padC[0x4];
    Model *model;
    char pad14[0x24];
    int x38;
    char pad3C[0x20];
    Handle x5C;
    char x6C[0x1A4];
    float x210;
    char pad214[0x20];
    float x234;
    char pad238[0x18];
    float x250;
} Obj;

typedef struct Game {
    char pad0[0x10C];
    int x10C;
    char pad110[0x190];
    int x2A0;
} Game;

typedef struct State {
    char pad0[0x5019C];
    float x5019C;
} State;

extern Game D_004B25A0;
extern State D_002AF2C0;
extern float D_0029C168[];

extern int func_0011C0D0(State *, Vec4 *, float, int);
extern int func_0011C168(State *, Vec4 *, float, float *);
extern int func_0011B180(State *, Vec4 *, float *, int, float);
extern void func_00204358(void *, int);
extern void func_0011E568(Obj *, int);
extern void func_0011F1E0(Obj *);
extern void func_0011F298(Obj *);
extern void func_0011F3B8(Obj *);
extern void func_0011F4C0(Obj *);
extern void func_0011F760(Obj *);
extern void func_0011F880(Obj *);
extern void func_0011F8C0(Obj *);
extern short func_00222BD8(int, int);
extern void func_00203DC8(Handle *, int);
extern void func_00203E10(Handle *, float);
extern void func_00202E38(Handle *, int, int, float);

void func_0011EDA8(Obj *obj)
{
    float range;
    float near;
    float maxDist;
    float dist;
    Handle handle;
    void *sound;

    if (D_004B25A0.x2A0 == 5 || D_004B25A0.x2A0 == 22) {
        range = 400.0f;
        near = 350.0f;
        maxDist = 90100.0f;
    } else {
        range = 300.0f;
        near = 250.0f;
        maxDist = 90000.0f;
    }
    if (obj->model == 0) {
        return;
    }
    obj->x250 = D_0029C168[0];
    if (CallShared0011C0D0_0(&D_002AF2C0, &obj->model->pos, range, 0)) {
        if ((unsigned int)(obj->type - 9) < 2 && D_004B25A0.x10C != 10) {
            obj->x38 = CallShared0011C168_1(&D_002AF2C0, &obj->model->pos, range, &obj->x250);
            obj->x250 = obj->x250 * obj->x250;
        } else {
            obj->x38 = func_0011B180(&D_002AF2C0, &obj->model->pos, &obj->x250, 0, near);
        }
        if (CallShared0011C168_2(&D_002AF2C0, &obj->model->pos, range, &dist)) {
            if (obj->type == 4 || obj->type == 9 || obj->type == 3) {
                dist *= 0.5f;
            }
            sound = obj->x6C;
            if (dist < 40.0f) {
                func_00204358(sound, 0);
            } else if (dist < 80.0f) {
                func_00204358(sound, 1);
            } else if (dist < 160.0f) {
                func_00204358(sound, 2);
            } else {
                func_00204358(sound, 3);
            }
        }
        if (obj->x38 != 0 && obj->x250 < maxDist) {
            switch (obj->type) {
            case 1:
                func_0011F1E0(obj);
                break;
            case 2:
                func_0011F298(obj);
                break;
            case 3:
                func_0011F3B8(obj);
                break;
            case 4:
                func_0011F4C0(obj);
                break;
            case 7:
                func_0011F760(obj);
                break;
            default:
                func_0011E568(obj, 5);
                break;
            case 10:
                func_0011F880(obj);
                break;
            case 11:
                func_0011F8C0(obj);
                break;
            }
        }
    } else if (D_002AF2C0.x5019C + 200.0f < obj->model->pos.z) {
        func_0011E568(obj, 9);
    } else if (obj->state == 5 && obj->type != 9) {
        if (func_00222BD8(0, 100) == 42) {
            func_0011E568(obj, 2);
        }
    } else if (obj->state == 4) {
        if (obj->x210 > 0.0f) {
            if (func_00222BD8(0, 100) == 42) {
                func_0011E568(obj, 2);
            }
        } else {
            handle = obj->x5C;
            func_00203DC8(&handle, 0);
            handle = obj->x5C;
            func_00203E10(&handle, obj->x234);
            func_00202E38(&obj->x5C, 1, 1, 600.0f);
            func_0011E568(obj, 1);
        }
    }
}
