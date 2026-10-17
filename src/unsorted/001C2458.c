/* UNIT 0x001C2458 0x001C27F8 .sdata=0x29D598:0x29D5A0 */
#include "scalar_api.h"
#include "particle.h"
typedef struct Scalar001C2458Obj {
    char pad0[0x2720];
    int x2720;
    char pad2724[2];
    unsigned short x2726;
    char pad2728[4];
    float x272C;
    char pad2730[0x28];
    unsigned int x2758;
    char pad275C[0x10];
    unsigned short x276C;
    char pad276E[0x9E];
    int x280C;
    char pad2810[0x18];
    unsigned int x2828;
    char pad282C[0x1C];
    void *x2848;
} Obj;

typedef struct Entry {
    int x0;
    char pad4[2];
    unsigned short x6;
    float x8;
    float xC;
    char pad10[0x28];
    unsigned int x38;
    char pad3C[4];
} Entry;

typedef struct T3Entry {
    char pad0[4];
    short x4;
    unsigned short x6;
} T3Entry;

float D_0029D598 = 0.5f;
float D_0029D59C = 0.4f;

extern unsigned int D_0027C478[];
extern int D_00270840[];
extern char D_00270830[];
extern Entry D_0027BFC0[];
extern T3Entry D_0038DB90[];
extern void func_0022DDD8(void *);
extern Obj *func_0022DD88(int);
extern Entry *func_00227DB8(int);

void func_001C2458(Obj **p1, Obj **p2, void *a2, void *a3, void *t0, void *t1, int t2, int t3, float f, float f2, unsigned int stk)
{
    Obj *o1;
    Obj *tmp1;
    Obj *tmp2c;
    Obj *tmp2u;
    Entry *e1;
    Entry *e2;
    int flag;
    int off;
    int idx;
    int id;
    unsigned int col1;
    void *t0arg;
    unsigned int col2;
    int hid;
    Obj *newobj;

    o1 = *p1;
    if (o1 != 0) {
        unsigned short tp = o1->x276C;
        if (tp == 0x2D || tp == 8 || tp == 0x27)
            return;
    }
    if (f < D_0029D59C)
        flag = 0xC00800;
    else
        flag = 0x400800;
    off = t3 << 3;
    if (o1 != 0) {
        if (o1->x2758 < D_0027C478[0] || t2 == 0 || !(((T3Entry *)((char *)D_0038DB90 + off))->x6 & 8)) {
            func_0022DDD8(*p1);
            *p1 = 0;
        }
    }
    if (t2 != 0 && (((T3Entry *)((char *)D_0038DB90 + off))->x6 & 8)) {
        col1 = (((stk >> 2) & 0x3F3F3F) + 0x202020) | 0x80000000;
        tmp1 = *p1;
        if (tmp1 == 0) {
            hid = func_0022CFB8((Vector4 *)(a2), a3, 0x12, (Vector4 *)(D_00270830), 1.0f, (Matrix4 *)(t0), (int)(1), (int)(col1));
            newobj = func_0022DD88(hid);
            *p1 = newobj;
            if (newobj != 0) {
                newobj->x280C = t3;
                (*p1)->x2848 = p1;
            }
        } else if (tmp1->x276C == 0x12) {
            (*p1)->x2720 = D_0027BFC0[0x12].x0 | flag | (tmp1->x2720 & 0x08000000);
            e1 = func_00227DB8(0x12);
            (*p1)->x2726 = e1->x6;
            (*p1)->x272C = D_0027BFC0[0x12].xC * f * D_0029D598;
            (*p1)->x2828 = col1;
        }
    }
    idx = ((T3Entry *)((char *)D_0038DB90 + off))->x4 + 1;
    id = D_00270840[idx];
    if (id == -1) {
        if (((T3Entry *)((char *)D_0038DB90 + off))->x6 & 1)
            id = 0x15;
    }
    tmp2c = *p2;
    if (tmp2c != 0) {
        if (id == -1 || tmp2c->x2758 < D_0027BFC0[id].x38 || t2 == 0) {
            func_0022DDD8(*p2);
            *p2 = 0;
        }
    }
    if (t2 == 0)
        return;
    if (id == -1)
        return;
    tmp2u = *p2;
    if (tmp2u == 0) {
        col2 = (stk & 0xFFFFFF) | 0x80000000;
        t0arg = (id == 0x15) ? t1 : t0;
        hid = func_0022CFB8((Vector4 *)(a2), a3, id, (Vector4 *)(D_00270830), 1.0f, (Matrix4 *)(t0arg), (int)(1), (int)(col2));
        newobj = func_0022DD88(hid);
        *p2 = newobj;
        if (newobj != 0) {
            newobj->x280C = t3;
            (*p2)->x2848 = p2;
        }
    } else {
        if (tmp2u->x276C != id)
            return;
        (*p2)->x2720 = D_0027BFC0[id].x0 | flag | (tmp2u->x2720 & 0x08000000);
        e2 = func_00227DB8(id);
        (*p2)->x2726 = e2->x6;
        (*p2)->x272C = D_0027BFC0[id].xC;
        (*p2)->x2828 = (stk & 0xFFFFFF) | 0x80000000;
    }
}
