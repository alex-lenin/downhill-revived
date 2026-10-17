/* UNIT 0x0015F0C8 0x0015F410 */
#include "menu_items.h"
typedef struct Entry {
    char pad[16];
} Entry;

typedef struct Sub {
    volatile short x0;
    volatile short x2;
    char pad4[3];
    unsigned char x7;
    char pad8[8];
} Sub;

typedef struct Obj {
    char pad0[0x18];
    Entry x18[64];
    int x418;
    char pad41C[0x1C];
    Sub sub1;
    Sub sub2;
    char pad458[8];
    int x460;
    volatile int x464;
    int x468;
    int x46C;
    int x470;
    char pad474[0x34];
    int x4A8;
    char pad4AC[0xF];
    unsigned char x4BB;
    char pad4BC[8];
    int x4C4;
    char pad4C8[0x38];
    int x500;
    int x504;
    int x508;
    int x50C;
} Obj;

typedef struct Manager {
    char pad0[0x20];
    int x20;
    char pad24[0x10C];
    int x130;
} Manager;

typedef struct Values {
    int x0;
    int x4;
    int x8;
    char padC[0x1B];
    unsigned char x27;
} Values;

typedef struct Glob {
    char pad0[0x11A60];
    Values vals;
    char pad11A88[0x712];
    unsigned char x1219A;
    char pad1219B[5];
    unsigned char x121A0;
} Glob;

typedef struct Res {
    char pad0[0x12C];
    char *x12C;
    char *x130;
    char *x134;
    char *x138;
} Res;

extern Manager D_003F52D0;
extern Glob D_004225A0;
/* long (size 8, like neighbor 0015F608.c): retail accesses this sbss resident
   with absolute lui+lw/la, not GP-relative; a size-4 pointer decl would emit
   GP-relative loads. Cast at use sites. */
extern long D_0029DE68;
extern char D_0038D010[];
/* Array-typed (indexed [0] use): retail emits absolute lui $v0 + lw $a1
   for these; plain extern int collapses to a GP-relative macro load. */
extern int D_0039E82C[];
extern int D_0039E830[];
extern void func_0023D650(int, int *, int *);
extern void func_0018E6B8(Obj *, int, Res **);
extern void func_0018E638(Obj *, int, Res **);
extern void func_0018D898(Entry *, int, Res **);
extern int func_002205A0(int);
extern int func_00220180(int);
extern void func_00191078(void *, void *, int, int, int, int);
extern void func_001B5AF0(void *, int, int);
extern void func_0015F4C8(void *, char *, int, int);

static inline Values *get_values(Glob *g) {
    return &g->vals;
}

/* Volatile read pins each resource-pointer load late (just before its call);
   a plain load is hoisted above the x504/x500/x50C divisions. */
static inline Res *get_res(void) {
    return *(volatile Res *volatile *)(volatile void *)&D_0029DE68;
}

void func_0015F0C8(Obj *obj) {
    /* volatile: retail reloads pos[0] for each use instead of CSE-sharing one
       load, so the loads must not be CSE-merged. */
    int pos[2];
    int half;
    int v500;
    Sub *s1;
    Sub *s2;
    int flag;

    func_0023D650(D_003F52D0.x20, (int *)&pos[0], (int *)&pos[1]);
    half = *(volatile int *)&pos[0] >> 1;
    obj->x460 = 0x54;
    obj->x468 = 10;
    obj->sub1.x0 = half;
    obj->x464 = half - 0x88;
    s1 = &obj->sub1;
    s1->x7 = 1;
    s1->x2 = 0x40;
    s2 = &obj->sub2;
    obj->sub2.x0 = *(volatile int *)&pos[0] >> 1;
    s2->x2 = 0xCC;
    s2->x7 = 1;
    flag = 1;
    obj->x4BB = flag;
    func_0018E6B8(obj, 0x10, (Res **)&D_0029DE68);
    obj->x46C = flag;
    obj->x4C4 = 0;
    obj->x4A8 = 5;
    obj->x470 = 0x14;
    func_0018E638(obj, 0x48, (Res **)&D_0029DE68);
    obj->x418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x4A, (Res **)&D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), get_values(&D_004225A0)->x27 == 0 ? 0x50 : 0x4F, (Res **)&D_0029DE68, 1);
    if (D_004225A0.x121A0 != 0) {
        int id = D_0039E82C[0];
        func_0018E3A8((struct SharedMenu *)(void *)(obj), id + 0x9B, (Res **)&D_0029DE68, 1);
    }
    if (D_004225A0.x1219A != 0) {
        int id2 = D_0039E830[0];
        func_0018E3A8((struct SharedMenu *)(void *)(obj), id2 + 0x9D, (Res **)&D_0029DE68, 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x4C, (Res **)&D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x4D, (Res **)&D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x4E, (Res **)&D_0029DE68, 1);
    if (func_00220180(func_002205A0(D_003F52D0.x130)) != 0) {
        func_0018D898(&obj->x18[0], 0x49, (Res **)&D_0029DE68);
    } else {
        func_0018D898(&obj->x18[0], 0x4A, (Res **)&D_0029DE68);
    }
    func_00191078(&D_003F52D0, D_0038D010, (pos[0] >> 1) - 0x62, 0xB4, 0xCC, 2);
    func_001B5AF0(&D_003F52D0, 1, 1);
    obj->x504 = get_values(&D_004225A0)->x4 / 8;
    v500 = get_values(&D_004225A0)->x8 / 8;
    obj->x500 = v500;
    obj->x50C = get_values(&D_004225A0)->x0 / 8;
    obj->x508 = 1;
    func_0015F4C8(obj, get_res()->x130, 0x11, v500);
    func_0015F4C8(obj, get_res()->x134, 0x11, obj->x50C);
    func_0015F4C8(obj, get_res()->x12C, 0x11, obj->x508);
    func_0015F4C8(obj, get_res()->x138, 0x11, obj->x504);
}
