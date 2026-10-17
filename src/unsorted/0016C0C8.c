/* UNIT 0x0016C0C8 0x0016C1D0 */
#include "timer.h"
typedef struct Game {
    char pad0[0x488];
    int x488;
} Game;

typedef struct Obj {
    int x0;
    int x4;
} Obj;

extern Game D_0043EEC0;
extern char D_004D0318[], D_002AF2C0[], D_00759780[];
extern unsigned char D_0043DD40[];
extern int D_0029DD70;
extern char D_0036B870[], D_00372650[], D_004BF950[], D_004C5C50[], D_00308D40[];
extern char D_004348F0[], D_0037EA60[];

extern void func_0019EE98(Game *);
extern void func_0016C1D0(Obj *);
extern void func_002210C0(void *);
extern void func_0011C5F8(void *);
extern void func_0011A100(void *);
extern void func_001638B0(void *);
extern void func_00143D20(void *);
extern void func_001472C8(void *);
extern void func_001DC548(void *);
extern void func_001EAD60(void *);
extern void func_00127738(void *);
extern void func_00196138(void *);
extern void func_0020CDF0(void);
extern void func_001482B0(void *);

void func_0016C0C8(Obj *obj)
{
    if (D_0043EEC0.x488 < 1 || D_0043EEC0.x488 > 3) {
        func_0019EE98(&D_0043EEC0);
        return;
    }
    if (obj->x4) {
        func_0016C1D0(obj);
    }
    func_002210C0(D_004D0318);
    func_0011C5F8(D_002AF2C0);
    func_0011A100(D_002AF2C0);
    if (!D_0043DD40[0]) {
        func_00236C60((TimerPool *)(D_00759780));
    }
    func_001638B0(&D_0029DD70);
    func_00143D20(D_0036B870);
    func_001472C8(D_00372650);
    func_001DC548(D_004BF950);
    func_001EAD60(D_004C5C50);
    func_00127738(D_00308D40);
    func_00196138(D_004348F0);
    func_0020CDF0();
    func_001482B0(D_0037EA60);
}
