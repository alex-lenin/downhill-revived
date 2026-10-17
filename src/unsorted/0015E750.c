/* UNIT 0x0015E750 0x0015ED18 */
typedef struct Entry { char pad0[0xC]; short xC; short xE; } Entry;
typedef struct Obj {
    int x0;
    char pad4[0xC];
    Entry entries[1];
    char pad20[0x4E0];
    int x500;
    int x504;
    int x508;
    int x50C;
    char pad510[0x14];
    int x524;
} Obj;
typedef struct Manager { char pad0[0x20]; int x20; char pad24[0x10C]; int x130; } Manager;
typedef struct Values { int x0; int x4; int x8; } Values;
typedef struct Glob { char pad0[0x11A60]; Values vals; } Glob;
extern Manager D_003F52D0;
extern Glob D_004225A0;
extern char D_004B2914[];
extern char D_0038D510[];
extern char D_0038D010[];
extern char D_00513B00[];
extern long D_0029DE68;
extern int D_003F52F0[];
extern char D_0028A440[];
extern void func_0018E6B8(Obj *, int, void *);
extern char *func_0018D860(void *);
extern void func_0018DC20(Obj *);
extern void func_001C6310(void *, int);
extern void func_001C6220(void *, int);
extern void func_001C6258(void *, int, int);
extern void func_001C65F0(void *, int);
extern void func_001C6558(void *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F9A8(int, int, int, int, void *);
extern void func_0023D650(int, int *, int *);
extern void func_001913A8(void *, void *, int, int, int, int);
extern void func_002103D8(void *, int, void *);
extern int func_002205A0(int);
extern int func_00220230(int);
extern int func_002209A8(int, int);
extern void func_0015F4C8(void *, char *, int, int);
static __inline__ int IsType(Entry *entry, short id) {
    return entry->xC == id;
}
static __inline__ int IsItem(Obj *obj, int index, short id) {
    return IsType(&obj->entries[index], id);
}
static __inline__ Values *get_values(Glob *g) {
    return &g->vals;
}
void func_0015E750(Obj *obj) {
    float scale;
    int pos[2];
    unsigned short buttons;
    int cx;
    func_0018E6B8(obj, 0xA6, &D_0029DE68);
    if (IsItem(obj, obj->x0, 0x4C)) {
        goto second;
    }
    if (IsItem(obj, obj->x0, 0x4E)) {
        goto second;
    }
    if (!IsItem(obj, obj->x0, 0x4D)) {
        goto check4B;
    }
second:
    scale = 1.0f;
    func_0018E6B8(obj, 0xAA, &D_0029DE68);
    if (IsItem(obj, obj->x0, 0x4C)) {
        scale = (float)obj->x500 * 0.0078125f;
        func_001C6310(D_004B2914, 0x4C);
    } else if (IsItem(obj, obj->x0, 0x4E)) {
        scale = (float)obj->x504 * 0.0078125f;
        func_001C6310(D_004B2914, 0x4E);
    } else if (IsItem(obj, obj->x0, 0x4D)) {
        scale = (float)obj->x50C * 0.0078125f;
        func_001C6310(D_004B2914, 0x4D);
    }
    func_0023D650(D_003F52D0.x20, &pos[0], &pos[1]);
    cx = (pos[0] >> 1) - 0x62;
    func_001913A8(&D_003F52D0, D_0038D510, (int)(scale * 200.0f) + cx, 0xB0, 4, 10);
    func_002103D8(D_00513B00, D_003F52D0.x20, D_0038D010);
    func_002103D8(D_00513B00, D_003F52D0.x20, D_0038D510);
    goto tail;
check4B:
    if (IsItem(obj, obj->x0, 0x4B)) {
        func_0020F518(0, 0x78, 0x28, 0x28, 0x80);
        func_0020F9A8(0, D_003F52F0[0], 0x9E, 0xB4, D_0028A440);
    } else {
        func_001C6558(D_004B2914);
    }
tail:
    func_0018DC20(obj);
    if (obj->x524 != 0) {
        return;
    }
    buttons = func_00220230(func_002205A0(D_003F52D0.x130));
    if ((buttons & 0x20) || func_002209A8(0x20, func_002205A0(D_003F52D0.x130))) {
        if (IsItem(obj, obj->x0, 0x4C)) {
            if (obj->x500 < 0x81) {
                obj->x500++;
                func_0015F4C8(obj, func_0018D860((char *)obj + (obj->x0 * 16 + 0x18)), 0x11, obj->x500);
                func_001C6220(D_004B2914, obj->x500 * 8);
                get_values(&D_004225A0)->x8 = obj->x500 * 8;
            }
        }
        if (IsItem(obj, obj->x0, 0x4E)) {
            if (obj->x504 < 0x81) {
                obj->x504++;
                func_0015F4C8(obj, func_0018D860((char *)obj + (obj->x0 * 16 + 0x18)), 0x11, obj->x504);
                func_001C6258(D_004B2914, obj->x504 * 8, 0);
                get_values(&D_004225A0)->x4 = obj->x504 * 8;
            }
        }
        if (IsItem(obj, obj->x0, 0x4D)) {
            if (obj->x50C < 0x81) {
                obj->x50C++;
                func_0015F4C8(obj, func_0018D860((char *)obj + (obj->x0 * 16 + 0x18)), 0x11, obj->x50C);
                func_001C65F0(D_004B2914, obj->x50C * 8);
                get_values(&D_004225A0)->x0 = obj->x50C * 8;
            }
        }
    }
    if ((buttons & 0x80) || func_002209A8(0x80, func_002205A0(D_003F52D0.x130))) {
        if (IsItem(obj, obj->x0, 0x4C)) {
            if (obj->x500 > 0) {
                obj->x500--;
                func_0015F4C8(obj, func_0018D860((char *)obj + (obj->x0 * 16 + 0x18)), 0x11, obj->x500);
                func_001C6220(D_004B2914, obj->x500 * 8);
                get_values(&D_004225A0)->x8 = obj->x500 * 8;
            }
        }
        if (IsItem(obj, obj->x0, 0x4E)) {
            if (obj->x504 > 0) {
                obj->x504--;
                func_0015F4C8(obj, func_0018D860((char *)obj + (obj->x0 * 16 + 0x18)), 0x11, obj->x504);
                func_001C6258(D_004B2914, obj->x504 * 8, 0);
                get_values(&D_004225A0)->x4 = obj->x504 * 8;
            }
        }
        if (IsItem(obj, obj->x0, 0x4D)) {
            if (obj->x50C > 0) {
                obj->x50C--;
                func_0015F4C8(obj, func_0018D860((char *)obj + (obj->x0 * 16 + 0x18)), 0x11, obj->x50C);
                func_001C65F0(D_004B2914, obj->x50C * 8);
                get_values(&D_004225A0)->x0 = obj->x50C * 8;
            }
        }
    }
}
