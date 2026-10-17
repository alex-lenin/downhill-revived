/* UNIT 0x00166F68 0x00167370 */
typedef struct Entry { char pad0[0xC]; short xC; short xE; } Entry;
typedef struct Obj {
    int x0;
    char pad4[0xC];
    Entry entries[1];
    char pad20[0x4E0];
    int x500;
    int x504;
    float x508, x50C, x510, x514, x518, x51C, x520, x524, x528;
} Obj;
typedef struct Menu { char pad0[0x20]; int x20; char pad24[0x10C]; int x130; } Menu;

extern Menu D_003F52D0;
extern long D_0029DE68;
extern char D_00396B40[];
extern char D_00396C80[];
extern char D_00513B00[];
extern void func_0018E6B8(Obj *, int, void *);
extern void func_0018DC20(Obj *);
extern void func_001913A8(Menu *, char *, int, int, int, int);
extern void func_002103D8(char *, int, void *);
extern int func_002205A0(int);
extern int func_00220230(int);
extern int func_002209A8(int, int);
#include "scene_effects.h"

static __inline__ int IsType(Entry *entry, short id) {
    return entry->xC == id;
}

static __inline__ int IsItem(Obj *obj, int index, short id) {
    return IsType(&obj->entries[index], id);
}

void func_00166F68(Obj *obj) {
    float *value;
    float t;
    unsigned short buttons;

    if (!IsItem(obj, obj->x0, 0x72)) {
        func_0018E6B8(obj, 0xAA, &D_0029DE68);
        if (IsItem(obj, obj->x0, 0x6D)) {
            value = &obj->x508;
        } else if (IsItem(obj, obj->x0, 0x6E)) {
            value = &obj->x50C;
        } else if (IsItem(obj, obj->x0, 0x6F)) {
            value = &obj->x514;
        } else if (IsItem(obj, obj->x0, 0x70)) {
            value = &obj->x518;
        } else if (IsItem(obj, obj->x0, 0x71)) {
            value = &obj->x51C;
        } else {
            value = 0;
        }
        if (value == 0) {
            return;
        }
        if (IsItem(obj, obj->x0, 0x6D)) {
            t = *value / (obj->x524 - 20.0f);
            if (t > 0.995f) {
                t = 0.995f;
            }
        } else if (IsItem(obj, obj->x0, 0x6E)) {
            t = (*value - 32.0f) * (1.0f / 224.0f);
        } else {
            t = *value * (1.0f / 256.0f);
        }
        func_001913A8(&D_003F52D0, D_00396B40, (int)(t * 140.0f) + 0xBE, 0x8A, 4, 10);
        func_002103D8(D_00513B00, D_003F52D0.x20, D_00396C80);
        func_002103D8(D_00513B00, D_003F52D0.x20, D_00396B40);
        func_0018DC20(obj);
        buttons = func_00220230(func_002205A0(D_003F52D0.x130));
        if (obj->x504) {
            return;
        }
        if ((buttons & 0x20) || func_002209A8(0x20, func_002205A0(D_003F52D0.x130))) {
            if (IsItem(obj, obj->x0, 0x6D)) {
                if (*value < obj->x524 + 6.0f - 1.0f) {
                    obj->x508 += 6.0f;
                }
            } else if (*value < 255.0f) {
                *value += 1.0f;
            }
        }
        if ((buttons & 0x80) || func_002209A8(0x80, func_002205A0(D_003F52D0.x130))) {
            if (IsItem(obj, obj->x0, 0x6D)) {
                if (14.0f < *value) {
                    obj->x508 -= 6.0f;
                }
            } else if (IsItem(obj, obj->x0, 0x6E)) {
                if (32.0f < *value) {
                    *value -= 1.0f;
                }
            } else if (1.0f < *value) {
                *value -= 1.0f;
            }
        }
        func_0020ECD0((int)obj->x514, (int)obj->x518, (int)obj->x51C, obj->x508, obj->x528, obj->x50C, obj->x510);
        obj->x500 = 1;
    } else {
        func_0018E6B8(obj, 0xA5, &D_0029DE68);
        func_0018DC20(obj);
    }
}
