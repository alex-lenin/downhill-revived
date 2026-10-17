/* UNIT 0x0011B588 0x0011B658 */
typedef struct Vec { float x,y,z; } Vec;
typedef struct Out { float x0; int x4; Vec x8,x14; } Out;
typedef struct Body { char pad0[0x10]; Vec x10; char pad1[0x34]; Vec x50; } Body;
typedef struct Rider { char pad0[0x792c]; Body *x792c; } Rider;
typedef struct World { char pad0[0x50130]; Rider *x50130; } World;
typedef struct State { char pad0[0x10]; Vec x10; char pad1[0x64]; Vec x80; char pad2[0x22c]; int x2b8; } State;
extern State D_0037EA60;
extern World D_002AF2C0;
int func_0011B588(Out *out) {
    Rider *r;
    State *state=&D_0037EA60;
    out->x4=1;
    out->x0=2.0f;
    if (state->x2b8 >=4) {
        Vec *v=&state->x80;
        out->x14.x=state->x10.x;
        out->x14.y=state->x10.y;
        out->x14.z=state->x10.z;
        out->x8.x=state->x80.x;
        out->x8.y=v->y;
        out->x8.z=v->z;
    } else {
        r=D_002AF2C0.x50130;
        if(r && r->x792c) {
            out->x14.x=r->x792c->x50.x;
            out->x14.y=r->x792c->x50.y;
            out->x14.z=r->x792c->x50.z;
            out->x8.x=r->x792c->x10.x;
            out->x8.y=r->x792c->x10.y;
            out->x8.z=r->x792c->x10.z;
        }
    }
    return 0x20;
}
