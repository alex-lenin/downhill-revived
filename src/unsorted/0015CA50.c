/* UNIT 0x0015CA50 0x0015CAE0 */
#include "menu.h"
struct Obj {
    char pad0[0x45C];
    int x45C;
    int x460;
    char pad464[0x14];
    int x478;
    char pad47C[0x24];
    int x4A0;
    int x4A4;
    char pad4A8[0x20];
    void *x4C8;
    char pad4CC[0x2C];
    void *x4F8;
    char pad4FC[0x8];
    void *x504;
};


extern void func_0018E638(struct Obj *, int, void *);
extern char D_00285618[];
extern char D_00287308[];
extern int D_0029DE68;

struct Obj *func_0015CA50(struct Obj *p) {
    void **sub;

    func_0018D9D0((SharedMenu *)(p));
    sub = &p->x504;
    p->x4F8 = D_00285618;
    *sub = D_00287308;
    p->x45C = 16;
    p->x4A0 = 2;
    p->x4A4 = 3;
    p->x460 = 0x46;
    func_0018E638(p, 0xF, &D_0029DE68);
    p->x4C8 = sub;
    p->x478 = 1;
    return p;
}
