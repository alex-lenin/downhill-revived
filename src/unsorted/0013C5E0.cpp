/* UNIT 0x0013C5E0 0x0013C6A8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vtable {
    char pad0[0x30];
    short delta;
    short pad32;
    void (*func)(void *);
} Vtable;

typedef struct Child {
    int x0;
    char pad4[0x4F4];
    Vtable *vt;
} Child;

typedef struct Obj {
    char pad0[0x45C];
    int x45C;
    char pad460[0xA0];
    Child *x500;
    char pad504[0x20];
    int x524;
} Obj;

typedef struct Global {
    char pad0[0x64];
    int x64;
} Global;

extern Global D_003F52D0;
extern char D_004B2A90[];
extern void func_0018E5A8(Child *);
extern void func_0018E518(Obj *);
extern int func_00190D60(void *, int, int);
extern void func_001C57A8(void *, u128);
extern void func_0013C400(Obj *, int);

void func_0013C5E0(Obj *p)
{
    Global *g = &D_003F52D0;
    int *q = &g->x64;
    Child *c;
    Vtable *v;

    if (p->x500 != 0 && p->x524 == 0) {
        p->x500->x0 = -1;
        func_0018E5A8(p->x500);
        c = p->x500;
        v = c->vt;
        v->func((char *)c + v->delta);
    }
    if (p->x524 == 0) {
        func_0018E518(p);
    }
    if (func_00190D60(q, 1, p->x45C)) {
        func_001C57A8(D_004B2A90, (u128)0x5458454E5F53UL);
        func_0013C400(p, 1);
    }
}
