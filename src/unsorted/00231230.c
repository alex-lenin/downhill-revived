/* UNIT 0x00231230 0x00231628 */
#include "load_queue.h"
typedef struct Buffer { int x0, x4; } Buffer;
typedef struct State { char pad0[0x1684]; Buffer *x1684; int x1688; } State;
typedef struct Info { int x0; unsigned int x4; char x8[16]; char x18[16]; } Info;

extern int D_0029E338;
extern int D_0029E33C;
extern int D_0029E348[2]; /* double-buffer semaphores */
extern int D_0029E350;
extern int D_0029E354;
extern int D_0029E35C;
extern int D_0029E360;
extern int D_0029E364;
extern int D_0029E370[2]; /* double-buffer addresses */
extern int D_00738E40[];
extern int D_00738F30[];
extern char D_00739020[][64];
extern int D_0073A178[];
extern int func_0025A5D0(void);
extern State *func_00230E80(int);
extern int func_0020D260(State *, char *);
extern int func_0025A720(int);
extern int func_0025A700(int);
extern int func_0020D2D8(State *, char *, unsigned char);
extern void func_002311C8(int);
extern int func_0020E520(Info *, char *);
extern int func_0020E808(int);
extern int func_002474E0(int);
extern int func_00247D28(int, int, int, char *);
extern int func_00247F08(void);
extern int func_00242240(int);
extern void func_00230FC8(void);
extern void func_0025A5B0(int);
extern void func_0020D0F8(State *, int, int, char *, int);

/* Loads every queued file (names in D_00739020, count D_0029E35C): files already resident
   (func_0020D260) are registered directly, others are streamed in 0x20000-byte chunks
   through the double buffer D_0029E370[D_0029E354], guarded by the semaphores in
   D_0029E348; the first chunk of each file also allocates and registers it.
   Matching notes: each branch increments D_0029E350 itself (so the loop test reloads it),
   func_0025A700 returns int (its $v0 result steers local register allocation),
   and `first = 0` comes after the func_0020D0F8 call. */
void func_00231230(void)
{
    Info info;
    char params[3];
    State *state;
    unsigned int i;
    unsigned int count;
    unsigned int size;
    int src;
    int first;
    int done;
    int addr;
    int len;
    int *bufs;

    params[0] = 20;
    params[1] = 1;
    params[2] = 0;
    D_0029E350 = 0;
    D_0029E338 = func_0025A5D0();
    state = func_00230E80(D_0029E360);
    while (D_0029E350 < D_0029E35C) {
        info.x0 = 0;
        info.x4 = 0;
        info.x8[0] = 0;
        info.x18[0] = 0;
        if (func_0020D260(state, D_00739020[D_0029E350]) == 0) {
            func_0025A720(D_0029E348[D_0029E354]);
            D_00738E40[D_0029E350] = func_0020D2D8(state, D_00739020[D_0029E350], D_0073A178[D_0029E350]);
            D_00738F30[D_0029E350] = 0;
            func_002311C8(50);
            func_0025A700(D_0029E348[D_0029E354]);
            D_0029E354 = !D_0029E354;
            D_0029E350++;
        } else {
            if (func_0020E520(&info, D_00739020[D_0029E350]) != 0) {
                count = ((info.x4 - 0x70) >> 17) + 1;
                D_00738F30[D_0029E350] = 1;
                first = 1;
                for (i = 0; i < count; i++) {
                    if ((i + 1) << 17 >= info.x4) {
                        size = info.x4 - (i << 17);
                    } else {
                        size = 0x20000;
                    }
                    src = info.x0 + func_0020E808(0x20000) * i;
                    func_002474E0(0);
                    func_0025A720(D_0029E348[D_0029E354]);
                    bufs = D_0029E370;
                    do {
                        if (func_00247D28(src, func_0020E808(size), bufs[D_0029E354], params) != 0) {
                            if (first && D_0029E350 == 0) {
                                func_0025A5B0(D_0029E33C);
                            }
                            while (func_002474E0(1) != 0) {
                                func_002311C8(50);
                            }
                            done = 0;
                            if (func_00247F08() == 0) {
                                done = 1;
                            }
                        } else {
                            done = 0;
                        }
                    } while (!done);
                    if (first) {
                        len = func_00242240(bufs[D_0029E354]);
                        D_0029E364 = len;
                        /* 64-byte aligned allocation from the heap, else the fixed address */
                        if (state->x1684 != 0) {
                            int n = (state->x1684->x4 + 0x3F) & ~0x3F;
                            state->x1684->x4 = n + len;
                            addr = n;
                        } else {
                            addr = state->x1688;
                        }
                        D_00738E40[D_0029E350] = addr;
                        func_0020D0F8(state, addr, D_0029E364, D_00739020[D_0029E350], D_0073A178[D_0029E350]);
                        first = 0;
                    }
                    func_0025A700(D_0029E348[D_0029E354]);
                    D_0029E354 = !D_0029E354;
                }
            }
            D_0029E350++;
        }
    }
    func_00230FC8();
}
