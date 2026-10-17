/* UNIT 0x0011B778 0x0011BFD0 .sdata=0x29C138:0x29C13C */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));
typedef struct Matrix { Vec4 row[4]; } Matrix;

typedef struct Entry { float x0; int index; float pos[3]; float vector[3]; } Entry;
typedef struct Buffer { Entry entries[8]; } Buffer;

typedef struct View {
    Matrix x0;
    char pad40[0x40];
    Vec4 x80;
    Vec4 x90;
    char padA0[0x218];
    int mode;
    char pad2BC[0x98];
    int x354;
    char pad358[0x28];
} View;

typedef struct Actor {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x20];
    Matrix x40;
} Actor;

typedef struct Owner {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    Actor *actor;
} Owner;

typedef struct State { char pad[0x50130]; Owner *owners[2]; } State;

extern int D_0043F9B0[];
extern Buffer D_002FF490;
extern View D_0037EA60[];
extern State D_002AF2C0;
extern char D_0039E840[];
int D_0029C138 = 0;

extern int func_00230C80(void *);
extern int func_00230C90(void *);
extern void func_0011B6E0(Entry *, Vec4 *, Vec4 *, Matrix *);
extern void func_00231A08(void *, Vec4 *);
extern void func_00231A78(void *, Matrix *);
extern void func_00231BD8(void *);
extern void func_00135100(void *);
extern void func_00148860(View *);
#if defined(__mips__)
extern float func_00211980(Vec4 *, Vec4 *, int, float);
#endif

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\tsq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
        : "=m"((dst).row[0]), "=m"((dst).row[1]), "=m"((dst).row[2]), "=m"((dst).row[3]) : "r"(src) : "$8", "$9", "$10", "$11")
#define VADD(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b))
#define VTRANS(dst, a, m) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(m))

static inline void Owner_SetView(Owner *owner, Vec4 *pos, Matrix *m)
{
    func_00231A08(owner->x40C0, pos);
    func_00231A78(owner->x40C0, m);
}

static inline void Owner_Finish(Owner *owner, Matrix *m)
{
    func_00231A78(owner->x40C0, m);
    func_00231BD8(owner->x40C0);
    func_00135100(owner->x40C0);
}

static inline void View_Activate(int player)
{
    View *view = &D_0037EA60[player];

    if (view->mode == 1) {
        func_00148860(view);
    }
    view->x354 = 1;
}

static inline View *GetView(int index) {return &D_0037EA60[index];}
static inline Owner *GetOwner(int index) {return D_002AF2C0.owners[index];}
static inline void *GetBody(Owner *r) {return (char *)r+0x40c0;}
static inline void Move(void *p,Vec4 *pos,Matrix *mat,View *state,int one) {func_00231A08(p,pos);func_00231A78(p,mat);if(state->mode==one) func_00148860(state);state->x354=one;}
extern void func_0011B658(Entry *, Vec4 *, Vec4 *, Matrix *);

int func_0011B778(Entry *a) {
 Vec4 pos,other; Matrix mat; Vec4 offset,dir; int result=0;int mode;Vec4 *dirptr;Owner *r2; Owner *r; void *p; void *p2; View *state;
 if(a->x0==1.0f) {func_0011B658(a,&pos,&other,&mat);result=0x590;}
 else if(a->x0==2.0f) {func_0011B6E0(a,&pos,&other,&mat);result=0x20;}
 if(GetView(0)->mode>=4) {COPY_VECTOR(GetView(0)->x80,&pos);COPY_VECTOR(GetView(0)->x90,&other);COPY_MATRIX(GetView(0)->x0,&mat);}
 else if((mode=D_0029C138)<=0) {
 r=GetOwner(0);
 if(r) {if(r->actor) {p=GetBody(r);Move(p,&pos,&mat,GetView(0),1);}}
 if(D_0029C138<0 && (r2=GetOwner(1))) {
 if(r2->actor) {
 dirptr=&dir;
 offset.x=8.0f;offset.y=0.0f;offset.z=0.0f;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(dir) : "r"(&offset),"r"(&r->actor->x40));
 COPY_VECTOR(offset,&r->actor->x10);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(offset) : "r"(&offset),"r"(dirptr));
 offset.z-=CallGroundProbe(&offset,dirptr,0,15.0f);
 p2=GetBody(r2);
 func_00231A08(p2,&offset);func_00231A78(p2,&mat);
 p=GetView(1);if(((View *)p)->mode==1) func_00148860(p);((View *)p)->x354=1;
 }}
 } else if(mode==1 && (p=GetOwner(1))) {if(((Owner *)p)->actor) {p=(char *)p+0x40c0;state=GetView(1);func_00231A08(p,&pos);func_00231A78(p,&mat);if(state->mode==mode) func_00148860(state);state->x354=mode;}}
 return result;
}

int func_0011BA48(Buffer *out) {
    int index = func_00230C90(D_0039E840);
    D_002FF490.entries[index].x0 = 2.0f;
    D_002FF490.entries[index].index = index;
    if (D_0037EA60[0].mode >= 4) {
        float *vector = &D_0037EA60[0].x80.x;
        D_002FF490.entries[index].vector[0] = D_0037EA60[0].x0.row[1].x;
        D_002FF490.entries[index].vector[1] = D_0037EA60[0].x0.row[1].y;
        D_002FF490.entries[index].vector[2] = D_0037EA60[0].x0.row[1].z;
        D_002FF490.entries[index].pos[0] = vector[0];
        D_002FF490.entries[index].pos[1] = vector[1];
        D_002FF490.entries[index].pos[2] = vector[2];
    } else if (D_002AF2C0.owners[0] && D_002AF2C0.owners[0]->actor) {
        Owner *owner = D_002AF2C0.owners[0];
        D_002FF490.entries[index].vector[0] = owner->actor->x40.row[1].x;
        D_002FF490.entries[index].vector[1] = owner->actor->x40.row[1].y;
        D_002FF490.entries[index].vector[2] = owner->actor->x40.row[1].z;
        D_002FF490.entries[index].pos[0] = owner->actor->x10.x;
        D_002FF490.entries[index].pos[1] = owner->actor->x10.y;
        D_002FF490.entries[index].pos[2] = owner->actor->x10.z;
    }
    *out = D_002FF490;
    return 256;
}

int func_0011BC18(Buffer *in)
{
    Vec4 pos;
    Vec4 dir;
    Matrix m;
    Vec4 a;
    Vec4 b;
    int index;
    Owner *o0;
    Owner *o1;
    Vec4 *normal;

    D_0043F9B0[0] = 0;
    index = func_00230C80(D_0039E840);
    if (func_00230C80(D_0039E840) < 0) {
        D_002FF490 = *in;
    } else if (D_002FF490.entries[index].index == index) {
        func_0011B6E0(&D_002FF490.entries[index], &pos, &dir, &m);
        if (D_0037EA60[0].mode >= 4) {
            COPY_VECTOR(D_0037EA60[0].x80, &pos);
            COPY_VECTOR(D_0037EA60[0].x90, &dir);
            COPY_MATRIX(D_0037EA60[0].x0, &m);
        } else if (D_0029C138 <= 0) {
            o0 = D_002AF2C0.owners[0];
            if (o0 != 0 && o0->actor != 0) {
                Owner_SetView(o0, &pos, &m);
                Owner_Finish(o0, &m);
                View_Activate(0);
            }
            if (D_0029C138 < 0) {
                Owner *other = D_002AF2C0.owners[1];

                if (other != 0 && other->actor != 0) {
                    normal = &b;
                    a.x = 8.0f;
                    a.y = 0.0f;
                    a.z = 0.0f;
                    VTRANS(b, &a, &o0->actor->x40);
                    COPY_VECTOR(a, &o0->actor->x10);
                    VADD(a, &a, normal);
                    a.z -= CallGroundProbe(&a, normal, 0, 15.0f);
                    Owner_SetView(other, &a, &m);
                    func_00231A78(o0->x40C0, &m);
                    func_00231BD8(o0->x40C0);
                    func_00135100(o0->x40C0);
                    View_Activate(1);
                }
            }
        } else if (D_0029C138 == 1) {
            o1 = D_002AF2C0.owners[1];
            if (o1 != 0 && o1->actor != 0) {
                Owner_SetView(o1, &pos, &m);
                View_Activate(1);
            }
        }
    }
    return 256;
}
