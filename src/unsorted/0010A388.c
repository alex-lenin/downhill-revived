/* UNIT 0x0010A388 0x0010A490 */
#include "timer.h"
#include "include_asm.h"

struct S { int x0, x4, x8; unsigned short xc, xe; int x10; };
struct V3 { float x0, x4, x8; };
extern void func_0010A528(int, int *, void *);
extern void func_002038E0(void *, int);
extern void func_00203590(void *, int, int, int);
extern char D_0037EA60[];
extern int func_00148560(void *, int);
extern int D_0043F348[];
extern char D_00759780[];
extern void func_00260868(void *);

void func_0010A388(void *a, void *b, struct S *s) {
    func_0010A528(s->x8, &s->x10, a);
}

void func_0010A3B0(struct S *s) {
    int buf[4];
    buf[2] = 0;
    func_00203590(buf, s->x8, s->xc & 0x7FF, s->xe);
    func_002038E0(buf, 1);
}

void func_0010A3F0(void *a, struct V3 *v) {
    D_0043F348[0] = 0;
    func_00148560(D_0037EA60, 7);
    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00260868), a, v->x8 * 60.0f);
}

void func_0010A468(void) {
    func_00148560(D_0037EA60, 1);
}
