/* UNIT 0x001534E0 0x00153938 */
typedef struct Packed { char data[16]; } __attribute__((packed)) Packed;
typedef struct Rider { char pad0[0x11A0]; int x11A0,x11A4,x11A8; char pad11AC[0xBC]; int x1268,x126C,x1270; char pad1274[0x167]; unsigned char x13DB,x13DC,x13DD; char pad13DE[0xA62]; char x1E40[0x4550]; int x6390; } Rider;
typedef struct State {
    Rider *rider;
    char pad4[4];
    float x8, xC, x10;
    int action;
    unsigned char ready;
    unsigned char active;
    unsigned char notified;
    char pad1B[0xD];
    int x28;
} State;
extern void func_0012E230(Rider *, int, int);
extern int func_0012E208(Rider *);
extern float func_00203C28(Packed *);
extern int func_00204230(Packed *);
static inline int A(Rider *r) { int value; if(r->x13DC) value=r->x126C; else value=r->x11A4; return value; }
static inline int B(Rider *r) { int value; if(r->x13DB) value=r->x1268; else value=r->x11A0; return value; }
static inline Packed Get(char *base,int index) { Packed *p=(Packed *)(base+(index*16+0x14)); return *p; }

int func_001534E0(State *state)
{
    Packed first, second;
    int action, flag, value;
    float f;
    Rider *r;
    char *base;

    if (state->x8 == 0.0f) {
        state->x8 = 1.2f;
        state->xC = 40.0f;
        state->x10 = 100.0f;
    }
    r = state->rider;
    base = r->x1E40;
    if (r->x6390 != 3)
        return 0;
    flag = 0;
    if (!state->action) {
        if (A(r))
            func_0012E230(r, 8, 0);
        else if (B(r))
            func_0012E230(r, 3, 0);
        flag = func_0012E208(r);
    }
    action = 0;
    if (!state->ready && !A(r) && !B(r))
        state->ready = 1;
    if (!state->action) {
        if (state->ready) {
            if (flag) {
                value = A(r);
                action = 0xAC;
                if (!value) { value = B(r); action = 0xAF; if (value == 0) action = 0; }
            } else {
                value = A(r);
                action = 0xAA;
                if (!value) { value = B(r); action = 0xAD; if (value == 0) action = 0; }
            }
        } else if (state->active) {
            if (flag) {
                value = A(r);
                action = 0xAC;
                if (!value) { value = B(r); action = 0xAF; if (value == 0) action = 0; }
            } else if (A(r)) {
                action = 0xAA;
                if (state->x28 == 0xAA) action = 0xAB;
            } else if (B(r)) {
                action = 0xAD;
                if (state->x28 == 0xAD) action = 0xAE;
            }
        }
    } else {
        first = Get(base, state->action);
        if (func_00204230(&first)) {
            action = state->action;
            second = Get(base, action);
            f = func_00203C28(&second);
            if (f > 0.35f) {
                if (A(r) && !flag) {
                    if (f > 0.65f) {
                        if (state->ready != 0) goto mark;
                        if (state->active != 0) goto mark;
                        if (state->action == 0xAD) goto mark;
                        action = 0xAA;
                    } else action = 0xAA;
                } else if (B(r)) {
                    if (f > 0.65f) {
                        if (state->ready != 0) goto mark;
                        if (state->active != 0) goto mark;
                        if (state->action == 0xAA) {
mark:                       action = 0; state->active = 1;
                        } else action = 0xAD;
                    } else action = 0xAD;
                }
                if (!state->notified) {
                    state->notified = 1;
                    if (state->action >= 0xAD && state->action <= 0xAF) func_0012E230(r, 3, 1);
                    else func_0012E230(r, 8, 1);
                }
            } else {
                if (state->ready && !state->active) {
                    if (A(r) || B(r)) state->active = 1;
                }
            }
        }
    }
    return action;
}
