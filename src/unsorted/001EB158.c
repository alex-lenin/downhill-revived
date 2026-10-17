/* UNIT 0x001EB158 0x001EB1D8 */
#include "timer.h"
typedef struct Obj {
    char pad0[0x2EC1];
    unsigned char x2EC1;
    char pad1[0x48B0 - 0x2EC2];
    int x48B0;
} Obj;

typedef struct State {
    Obj *x0;
    int x4;
    unsigned char x8;
} State;

extern void func_001ACFA0(void *, int);
extern void func_00136030(void *, int);
extern char D_00759780[];
extern void func_00262700(void);

static inline void SetX2EC1(Obj *obj, int value) {
    obj->x2EC1 = value;
}

void func_001EB158(State *state) {
    Obj *obj = state->x0;
    SetX2EC1(obj, 1);
    func_001ACFA0((char *)obj + 0x4580, 0);
    obj->x48B0 = 0;
    func_00136030((char *)obj + 0x40C0, 1);
    if (!state->x8) {
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00262700), state, 1);
        state->x8 = 1;
    }
}
