/* UNIT 0x0017FA30 0x0017FB18 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Slot {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
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
    Slot x10[6];
    int xD0;
    int xD4;
    Handle xD8;
    char padE4[0xC];
    char xF0[0x50];
    Matrix x140;
    Vec4 x180;
    Vec4 x190;
    int x1A0;
    int x1A4;
    int x1A8;
    int x1AC;
    int x1B0;
    int x1B4;
    char x1B8;
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

void func_0017FA30(Obj *obj)
{
    int i;

    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    for (i = 5; i >= 0; i--) {
        obj->x10[i].x0 = 0;
    }
    obj->xD0 = 0;
    obj->xD4 = 0;
    InitHandle(&obj->xD8);
    func_002608C8(obj->xF0);
    InitMatrix(&obj->x140);
    InitAxis(&obj->x180);
    InitAxis(&obj->x190);
    obj->x1A0 = 0;
    obj->x1A4 = 0;
    obj->x1A8 = 0;
    obj->x1AC = 0;
    obj->x1B0 = 0;
    obj->x1B8 = 0;
    obj->x1B4 = 0;
}
