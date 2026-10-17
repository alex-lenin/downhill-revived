/* UNIT 0x001983B0 0x00198508 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Item {
    char pad0[0xC];
    short xC;
    short xE;
} Item;

typedef struct Obj {
    char pad0[0x10];
    Item x10[1];
} Obj;

typedef struct Menu {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
} Menu;

typedef struct Target {
    char pad0[0x10];
    Vec pos;
    char pad20[0x20];
    Vec mtx[3];
} Target;

typedef struct World {
    char pad0[0x792C];
    Target *x792C;
} World;

typedef struct Global {
    char pad0[0x12D];
    char x12D;
} Global;

extern World D_002AF2C0;
extern char D_004348F0[];
extern Menu D_00435500;
extern Menu D_00437340;
extern char D_004B28FC[];
extern Global D_003F52D0;
extern char D_003F53FD[];
extern int func_001961D8(void *, int, Vec *);
extern void func_00196388(void *, int, int);
extern void func_00190688(void *);

static inline int IsType(Item *item, int type) {
    return item->xC == type;
}

void func_001983B0(Menu *menu, int index)
{
    int type;
    Target *target;
    Vec ofs;
    Vec pos;
    Obj *obj;
    World *world;

    obj = (Obj *)menu;
    world = &D_002AF2C0;
    type = 5;
    if (IsType(&obj->x10[index], 0x62)) {
        type = 5;
    } else if (IsType(&obj->x10[index], 0x63)) {
        type = 2;
    } else if (IsType(&obj->x10[index], 0x64)) {
        type = 0;
    } else if (IsType(&obj->x10[index], 0x65)) {
        type = 3;
    } else if (IsType(&obj->x10[index], 0x66)) {
        type = 1;
    }

    if (menu->x500 == 0) {
        target = world->x792C;
        pos = (Vec){ 0.0f, 5.0f, 0.0f, 0.0f };
        __asm__ volatile ("lqc2 $vf11,0x0(%1)\n\tlqc2 $vf12,0x0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0x0(%0)" : : "r"(&ofs), "r"(&pos), "r"(target->mtx) : "memory");
        __asm__ volatile ("lqc2 $vf11,0x0(%1)\n\tlqc2 $vf12,0x0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(pos) : "r"(&target->pos), "r"(&ofs) : "memory");
        func_001961D8(D_004348F0, type, &pos);
        D_00437340.x500 = -1;
        D_00437340.x508 = -1;
        D_00437340.x504 = -1;
        D_004B28FC[0] = 1;
        D_003F53FD[0] = 1;
    } else {
        func_00196388(D_004348F0, menu->x504, type);
        D_00437340.x504 = type;
        D_00435500.x504 = type;
        func_00190688(&D_003F52D0);
    }
}
