/* UNIT 0x00152918 0x00152C58 */
typedef struct Packed { char data[16]; } __attribute__((packed)) Packed;
typedef struct Rider { char pad0[0x11A0]; int x11A0,x11A4,x11A8; char pad11AC[0xBC]; int x1268,x126C,x1270; char pad1274[0x167]; unsigned char x13DB,x13DC,x13DD; char pad13DE[0xA62]; char x1E40[0x4550]; int x6390; char pad6394[0x796C-0x6394]; int x796C; } Rider;
typedef struct State { Rider *rider; char pad4[0x10]; int action; unsigned char ready; unsigned char active; unsigned char notified; } State;
extern void func_0012E230(Rider *,int,int);
extern float func_00203C28(Packed *);
extern int func_00204230(Packed *);
static inline int A(Rider *r) { int value; if(r->x13DC) value=r->x126C; else value=r->x11A4; return value; }
static inline int B(Rider *r) { int value; if(r->x13DB) value=r->x1268; else value=r->x11A0; return value; }
static inline int C(Rider *r) { int value; if(r->x13DD) value=r->x1270; else value=r->x11A8; return value; }
static inline Packed Get(char *base,int index) { Packed *p=(Packed *)(base+(index*16+0x14)); return *p; }
int func_00152918(State *state) {
    Packed first,second;
    int action=0;
    int value;
    float f;
    Rider *r=state->rider;
    char *base=r->x1E40;
    if(r->x6390!=0) { state->active=0; goto finish; }
    if(C(r)) { state->active=0; action=0; goto finish; }
    if(!state->ready && !A(r) && !B(r)) state->ready=1;
    if(!state->action) {
        if(state->ready!=0) goto choose;
        if(state->active!=0) {
choose: {
            value=A(r);
            action=0x14;
            if(!value) { value=B(r); action=0x15; if(value==0) action=0; } }
        }
    } else {
        first=Get(base,state->action);
        if(func_00204230(&first)) {
            action=state->action;
            second=Get(base,action);
            f=func_00203C28(&second);
            if(f>0.35f) {
                value=A(r);
                if(value) {
                    if(f>0.65f) {
                        if(state->ready!=0) goto markA;
                        if(state->active!=0) goto markA;
                        if(state->action==0x15) {
markA:                      action=0; state->active=1;
                        } else action=0x14;
                    } else action=0x14;
                } else {
                    value=B(r);
                    if(value) {
                        if(f>0.65f) {
                            if(state->ready!=0) goto markB;
                            if(state->active!=0) goto markB;
                            if(state->action==0x14) {
markB:                          action=0; state->active=1;
                            } else action=0x15;
                        } else action=0x15;
                    }
                }
                if(!state->notified) {
                    state->notified=1;
                    if(state->action==0x15 || state->action==0x86) func_0012E230(r,0,1);
                    else func_0012E230(r,5,1);
                }
            } else {
                if(state->ready && !state->active) {
                    if(A(r) || B(r)) state->active=1;
                }
            }
        }
    }
finish:
    value=r->x796C;
    if(value==0x80 || value==0xC0) {
        if(action==0x15) action=0x86;
        else if(action==0x14) action=0x85;
    }
    return action;
}
