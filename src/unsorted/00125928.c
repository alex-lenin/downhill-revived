/* UNIT 0x00125928 0x00125A28 */
typedef struct Pair { int x[2]; } Pair;

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Node {
    long x0;
    char pad8[0x28];
    Vec x30;
} Node;

typedef struct VEntry {
    short delta;
    short pad2;
    Node *(*fn)(void *);
} VEntry;

typedef struct VObj {
    VEntry *vtbl;
} VObj;

typedef struct Sub {
    char pad0[0x48];
    float x48;
    int x4C;
} Sub;

typedef struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x34];
    int x40, x44, x48, x4C;
    char pad50[0x7C];
    float xCC;
    char padD0[0x60];
    VObj *x130;
    int x134;
    char pad138[0x14];
    Pair x14C, x154;
    char pad15C[0x24];
    Sub x180;
} Obj;

extern Pair D_0029C1E8[], D_0029C1F0[];
extern float func_00222B68(float, float);

void func_00125928(Obj *obj) {
    VObj *v;
    VEntry *e;
    Node *node;
    Vec scale;
    Sub *sub;

    obj->x14C = D_0029C1E8[0];
    obj->x154 = D_0029C1F0[0];
    obj->x8 = 1;
    sub = &obj->x180;
    sub->x48 = 0.25f;
    sub->x4C = 0;
    obj->x40 = 1;
    obj->x44 = 1;
    obj->x48 = 3;
    obj->x4C = 4;
    obj->xCC = func_00222B68(-0.34906584f, 0.34906584f);
    v = obj->x130;
    e = &v->vtbl[4];
    node = e->fn((char *)v + e->delta);
    if (node != 0) {
        node->x0 |= 1L << 56;
        scale.x = 1.4f;
        scale.y = 1.4f;
        scale.z = 1.4f;
        node->x30 = scale;
    }
    obj->x134 = 0;
}
