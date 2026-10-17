/* UNIT 0x001B99B8 0x001B9AE8 */
typedef struct Triple {
    int x0;
    int x4;
    int x8;
} Triple;

typedef struct Player {
    char pad0[0x694];
} Player;

typedef struct Obj {
    char pad0[0x10C];
    int x10C;
    char pad110[0x11C - 0x110];
    Triple x11C;
    char pad128[0x14C - 0x128];
    Triple x14C[(0x290 - 0x14C) / 12];
    int x290;
    char pad294[0x2A4 - 0x294];
    int x2A4;
} Obj;

extern Player D_004225A0[];
extern Triple D_00270748[];
extern int func_00194C80(Player *player, int mode, int index);
extern void func_001F96E8(Triple *dst, int value);
extern void func_001B9940(Obj *obj, int arg, int index);

static inline void SetTriple(Triple *t, int a, int b, int c) {
    t->x0 = a;
    t->x4 = b;
    t->x8 = c;
}

static inline Player *GetPlayer(int i) {
    return &D_004225A0[i];
}

void func_001B99B8(Obj *obj, int index) {
    int mode;
    int k;
    Triple *slot;
    Triple *dst;

    mode = obj->x10C;
    if (mode < 5) {
        func_001F96E8(&obj->x14C[index], func_00194C80(GetPlayer((obj->x290 >> 5) - 1), mode, index));
    } else if (mode == 10) {
        k = index + (obj->x2A4 & 1) * 8;
        dst = &obj->x14C[index];
        dst->x0 = D_00270748[k].x0;
        dst->x4 = D_00270748[k].x4;
        dst->x8 = D_00270748[k].x8;
        SetTriple(&obj->x11C, 0x11, 4, 0);
    } else if ((unsigned int)(obj->x11C.x0 - 0x23) < 3) {
        slot = &obj->x14C[index];
        slot->x0 = 0x11;
        slot->x4 = 4;
        slot->x8 = 0;
    } else {
        func_001B9940(obj, 0, index);
    }
}
