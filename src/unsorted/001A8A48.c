/* UNIT 0x001A8A48 0x001A8DE8 .sdata=0x29D3B0:0x29D3C8 */
#include "include_asm.h"

typedef struct Sub { char pad0[0x40]; unsigned char x40; } Sub;
typedef struct Obj {
    char pad0[8];
    int x8;
    int xC;
    void *x10, *x14;
    char pad18[8]; int x20; char pad24[4];
    int x28;
    int x2C;
    int x30;
    int x34;
    char pad38[0x44 - 0x38];
    char x44;
    char pad45[2];
    char x47;
    char pad48;
    char x49;
    char pad4A[0x54 - 0x4A];
    int x54;
    char pad58[0x9BC - 0x58];
    int x9BC;
    int x9C0;
} Obj;
typedef struct Game { char pad0[0x1B8]; int x1B8[3]; char pad1C4[0x24]; int x1E8[27]; int x254; char pad258[0xC]; int x264[9]; int x288; char pad28C[0x14]; int x2A0; } Game;

int D_0029D3B0 = 0;
int tail_0029D3B4 = 0;
int tail_0029D3B8 = 0x706572;
int tail_0029D3BC = 0;
int tail_0029D3C0 = 0x706572;
int tail_0029D3C4 = 0;
extern char D_004412C0[];
extern int D_0039E834[];
extern int D_0029DE78;
extern Game D_004B25A0;
extern void func_001AA128(Obj *);
extern void func_001AA100(Obj *);
extern void func_001A9038(Obj *);
extern void func_0023D898(int, float *, float *);
extern void func_0023D8D8(int, float, float);
extern void func_00220018(int, int, int, int);
extern void func_00220198(int, int);

void func_001A8A48(Obj *p) {
    func_001AA128(p);
    p->x44 = 0;
    p->x49 = 0;
    p->x10 = D_004412C0;
    p->x14 = D_004412C0;
    func_001A9038(p);
    p->x8 = 3;
    D_0029D3B0 = 0;
}

typedef struct Vec3 {
    int x, y, z;
} Vec3;

typedef struct Packet {
    unsigned char x0;
    unsigned char x1;
    unsigned short x2;
    char pad4[4];
    int x8;
    char padC[4];
    unsigned char x10;
    unsigned char x11;
    unsigned short x12[9];
    Vec3 x24;
    Vec3 x30;
    int x3C;
    unsigned char x40;
    Vec3 x44[9];
    Vec3 xB0[9];
    int x11C[9];
    int x140[1];
} Packet;

typedef struct Rider {
    char pad0[0x796C];
    volatile int x796C;
    char pad7970[0xEC];
    int x7A5C;
    char pad7A60[0x170];
} Rider;

typedef struct GameState {
    char pad0[0x1B8];
    Vec3 x1B8;
    char pad1C4[0x24];
    Vec3 x1E8[9];
    int x254;
    char pad258[0xC];
    int x264[9];
    int x288;
    char pad28C[0x14];
    int x2A0;
} GameState;

#define GAME (*(GameState *)&D_004B25A0)

typedef struct RiderTable {
    Rider riders[10];
    char pad4D620[0x501A8 - 10 * sizeof(Rider)];
    int count;
} RiderTable;

extern RiderTable D_002AF2C0;
extern struct { char pad0[0x5BB4]; int x5BB4; char pad5BB8[0x24]; int x5BDC; char pad5BE0[0x14]; int x5BF4; } D_00398C40;
extern void func_001B9860(GameState *game, int i, int *x, int *y, int *z);
extern void func_001B9910(GameState *game, int i, int *x, int *y, int *z);

#define PACKET ((Packet *)o->x10)

void func_001A8A98(Obj *o) {
    int i, j, mode, count, a, b, id, off;
    int *p, *end;
    Rider *r;
    Vec3 *v;

    j = 0;
    mode = GAME.x2A0;
    PACKET->x0 = mode;
    count = D_002AF2C0.count - GAME.x288;
    PACKET->x1 = count;
    PACKET->x8 = o->x20;
    PACKET->x40 = D_00398C40.x5BF4 != 0;
    a = D_00398C40.x5BB4;
    PACKET->x10 = a;
    b = D_00398C40.x5BDC;
    PACKET->x11 = b;
    for (i = 0; i < D_002AF2C0.count; i++) {
        r = &D_002AF2C0.riders[i];
        if (r->x7A5C == 1) {
            PACKET->x2 = r->x796C;
            func_001B9860(&GAME, 0, &PACKET->x24.x, &PACKET->x24.y, &PACKET->x24.z);
            v = &GAME.x1B8;
            PACKET->x30.x = v->x;
            PACKET->x30.y = v->y;
            PACKET->x30.z = v->z;
            id = GAME.x254;
            PACKET->x3C = id;
        } else if (r->x7A5C == 3) {
            PACKET->x12[j] = r->x796C;
            func_001B9910(&GAME, j, &PACKET->x44[j].x, &PACKET->x44[j].y, &PACKET->x44[j].z);
            /* addr of GAME.x1E8[j]; temp keeps (j*12+0x1E8)+GAME order */
            off = j * sizeof(Vec3) + 0x1E8;
            v = (Vec3 *)((char *)&GAME + off);
            PACKET->xB0[j].x = v->x;
            PACKET->xB0[j].y = v->y;
            PACKET->xB0[j].z = v->z;
            PACKET->x11C[j] = GAME.x264[j];
            j++;
        }
    }
    for (p = PACKET->x140, end = PACKET->x140 + 1; p < end; p++) {
        *p = 0;
    }
}

void func_001A8CE8(Obj *obj)
{
    float x, y;
    int i;

    func_001AA100(obj);
    D_0039E834[0] = ((Sub *)obj->x10)->x40;
    func_001A9038(obj);
    obj->x8 = 0;
    obj->x54 = 0;
    obj->x9BC = 0;
    obj->x44 = 1;
    obj->x9C0 = -1;
    func_0023D898(0, &x, &y);
    func_0023D8D8(0, 277.0f, y);
    obj->x30 = 1;
    D_0029D3B0 = 0;
    obj->x28 = 0;
    obj->x47 = 0;
    obj->x34 = 0;
    obj->x2C = 0;
    for (i = 0; i < D_004B25A0.x288; i++) {
        func_00220018(i, 0, 0, 0);
        func_00220198(i, 0);
    }
    D_0029DE78 = 0;
}
