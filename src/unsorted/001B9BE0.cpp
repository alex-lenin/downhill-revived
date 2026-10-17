/* UNIT 0x001B9BE0 0x001B9E28 */
#include "load_queue.h"
typedef struct Arena { int x0; int *x4; int *x8; } Arena;
typedef struct Context { char pad[0x1680]; int x1680; Arena *x1684; } Context;
extern Arena D_004D0368, D_00663B08;
extern Context D_004D30B0, D_004D4748, D_004D0380;
extern Arena *D_004D309C[];
extern char D_0029D490[], D_0029D498[], D_0029D468[], D_0029D478[], D_0029D488[];
extern void func_00230EC0(void *, int);
extern void func_00230F80(void *, void *, void *, void *, int, int);

extern int func_001BA6B8(void *);
extern int func_0020D170(Context *, int);
extern int func_00218E88(int, int);
extern void func_00218D98(int);
extern void func_0020D078(Context *, int);
extern void func_0020B228(int, int, int);
struct Final { void func_00158680(int, int); };
extern Final D_0029DD68;
void func_001B9BE0(void *arg0) {
 char a[0x150], b[0x150], c[0x150];
 int count, handle;
 int *old, *p;
 int last;
 count = D_004D4748.x1680;
 { int old; int p;
 old = (int)D_004D0368.x8;
 p = (((int)D_004D0368.x4 + 3) & ~3);
 D_004D0368.x8 = (int *)p;
 D_004D0368.x4 = (int *)(p + 4);
 *(int *)p = old;
 }
 D_004D30B0.x1684 = &D_004D0368;
 func_00230EC0(a, 1);
 func_00230F80(a, D_0029D490, D_0029D498, D_0029D468, 0, 0);
 ProcessLoadQueueLegacy(a);
 handle = func_001BA6B8(arg0);
 *(int *)((char *)arg0 + 0xB18C) = func_00218E88(handle, func_0020D170(&D_004D30B0, count));
 p = D_004D0368.x8;
 D_004D0368.x4 = p;
 old = (int *)*p;
 D_004D309C[0] = &D_004D0368;
 D_004D0368.x8 = old;
 func_00230EC0(b, 2);
 func_00230F80(b, D_0029D490, D_0029D498, D_0029D478, 0, 0);
 ProcessLoadQueueLegacy(b);
 func_00218D98(func_001BA6B8(arg0));
 { int old; int p;
 old = (int)D_00663B08.x8;
 p = (((int)D_00663B08.x4 + 3) & ~3);
 D_00663B08.x8 = (int *)p;
 D_00663B08.x4 = (int *)(p + 4);
 *(int *)p = old;
 }
 D_004D4748.x1684 = &D_004D0368;
 func_0020D078(&D_004D4748, 1);
 func_00230EC0(c, 3);
 func_00230F80(c, D_0029D490, D_0029D498, D_0029D488, 0, 0);
 ProcessLoadQueueLegacy(c);
 last = func_0020D170(&D_004D4748, count);
 func_0020B228(last, func_001BA6B8(arg0), count);
 D_00663B08.x4 = D_00663B08.x8;
 D_00663B08.x8 = (int *)*D_00663B08.x8;
 last = func_0020D170(&D_004D0380, 0);
 D_0029DD68.func_00158680(last, 0);
}
