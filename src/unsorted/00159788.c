/* UNIT 0x00159788 0x001598F0 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Slot {
    int x0;
    int x4;
    int x8;
    int xC;
} Slot;

typedef struct Handle {
    int x0;
    int x4;
    short x8;
    short xA;
} Handle;

typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct Matrix {
    u128 x0;
    u128 x10;
    u128 x20;
    float x30, x34, x38, x3C;
} Matrix;

typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    float x10[4];
    Slot x20[11];
    char padD0[8];
    Handle xD8;
    char padE4[0xC];
    char xF0[0x50];
    Matrix x140;
    int x180;
    char pad184[0xC];
    Vec4 x190;
    Vec4 x1A0;
    int x1B0;
    int x1B4;
    float x1B8;
    float x1BC;
    float x1C0;
    float x1C4;
    float x1C8;
    float x1CC;
    float x1D0;
    float x1D4;
    float x1D8;
    int x1DC;
    short x1E0;
    char x1E2;
    char x1E3;
    short x1E4;
    short x1E6;
    short x1E8;
    short x1EA;
    short x1EC;
    short x1EE;
} Obj;

extern void func_002608C8(void *);

static inline void InitHandle(Handle *h)
{
    h->x0 = 0;
    h->x4 = 0;
    h->xA = 0;
    h->x8 = 0;
}

static inline void ClearQuad(u128 *q)
{
    __asm__ volatile ("sq $0, %0" : "=m"(*q));
}

static inline void InitMatrix(Matrix *m)
{
    ClearQuad(&m->x0);
    ClearQuad(&m->x10);
    ClearQuad(&m->x10);
    m->x3C = 0;
    m->x38 = 0;
    m->x34 = 0;
    m->x30 = 0;
}

static inline void InitAxis(Vec4 *v)
{
    v->x = 0;
    v->y = 0;
    v->z = 1.0f;
    v->w = 0;
}

void func_00159788(Obj *obj)
{
    int i;

    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x10));
    obj->x20[0].x0 = 0;
    obj->x20[0].x4 = 0;
    obj->x20[0].x8 = 0;
    obj->x20[0].xC = 0;
    obj->x20[1].x8 = 0;
    obj->x20[1].xC = 0;
    obj->x20[2].x8 = 0;
    obj->x20[2].xC = 0;
    obj->x20[3].x8 = 0;
    obj->x20[3].xC = 0;
    obj->x20[4].x8 = 0;
    obj->x20[4].xC = 0;
    obj->x20[5].x8 = 0;
    obj->x20[5].xC = 0;
    obj->x20[6].x8 = 0;
    obj->x20[6].xC = 0;
    obj->x20[7].x8 = 0;
    obj->x20[7].xC = 0;
    obj->x20[8].x8 = 0;
    obj->x20[8].xC = 0;
    obj->x20[9].x8 = 0;
    obj->x20[9].xC = 0;
    obj->x20[10].x8 = 0;
    obj->x20[10].xC = 0;
    InitHandle(&obj->xD8);
    func_002608C8(obj->xF0);
    InitMatrix(&obj->x140);
    obj->x180 = 0;
    InitAxis(&obj->x190);
    InitAxis(&obj->x1A0);
    obj->x1B8 = 1.0f;
    obj->x1BC = 1.0f;
    obj->x1C0 = 1.0f;
    obj->x1C4 = 2.0f;
    obj->x1C8 = 1.0f;
    obj->x1CC = 1.0f;
    obj->x1D0 = 1.0f;
    obj->x1D4 = 1.5f;
    obj->x1D8 = 1.0f;
    obj->x1B0 = 0;
    obj->x1B4 = 0;
    obj->x1DC = 0;
    obj->x1E0 = 0;
    obj->x1E2 = 0;
    obj->x1E3 = 0;
    obj->x1E4 = 0;
    obj->x1E6 = 0;
    obj->x1E8 = 0;
    obj->x1EA = 0;
    obj->x1EC = 0;
    obj->x1EE = 0;
}
