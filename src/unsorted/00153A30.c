/* UNIT 0x00153A30 0x00153FE8 .sdata=0x29C658:0x29C674 */
#include "tween.h"
#include "include_asm.h"
typedef unsigned int u128 __attribute__((mode(TI)));
struct Vec { float x,y,z,w; };
struct Mat { u128 row[4]; };
struct Packed { long a,b; } __attribute__((packed));
struct Pose { char pad[16]; u128 pos; char pad20[32]; struct Mat mat; };
struct Parent { char pad[0x792c]; struct Pose *x792c; };
struct Obj { struct Parent *x0; char pad4[16]; int x14; char pad18[16]; struct Pose *x28; int x2c; char pad30[0x60]; int x90; };
unsigned int *D_0029C658=0;
float D_0029C65C=0,D_0029C660=90,D_0029C664=0,D_0029C668=2,D_0029C66C=1.5f,D_0029C670=4;
extern u128 D_007685D0[];
extern struct Mat D_00769CB0[];
extern int func_001B1C08(void *);
extern void func_00222040(void *,void *);
extern void func_00221930(void *,void *,void *);
extern float func_00203C28(void *);
extern void func_00154A70(void *),func_00154B90(void *),func_001524D8(void *,int);
extern int func_00238F48(void);
extern void *memcpy(void *,const void *,unsigned int);
#define COPY_VECTOR(d,s) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(d):"r"(s):"$2")
#define COPY_MATRIX(d,s) __asm__ volatile("lq $8,0(%4)\n\tlq $9,16(%4)\n\tlq $10,32(%4)\n\tlq $11,48(%4)\n\tsq $8,%0\n\tsq $9,%1\n\tsq $10,%2\n\tsq $11,%3":"=m"((d).row[0]),"=m"((d).row[1]),"=m"((d).row[2]),"=m"((d).row[3]):"r"(s):"$8","$9","$10","$11")

struct InitObj { char pad[8]; float x8, xC, x10; char pad14[0xC]; char x20, x21; char pad22[0xA]; int x2C; char pad30[0x68]; int x98, x9C; };
extern void func_001524B0(struct InitObj *, int);
void func_00153A30(struct InitObj *obj, int value) {
 func_001524B0(obj, value);
 obj->x8=1.2f;
 obj->xC=40.0f;
 obj->x10=100.0f;
 obj->x2C=-1;
 obj->x20=1;
 obj->x98=0;
 obj->x9C=0;
 obj->x21=0;
 D_0029C658=0;
}

void func_00153AA0(unsigned int *v) { D_0029C658=v; }
typedef struct Rider { char pad0[0x11A0]; int x11A0,x11A4,x11A8; char pad11AC[0xBC]; int x1268,x126C,x1270; char pad1274[0x167]; unsigned char x13DB,x13DC,x13DD; char pad13DE[0xA62]; char x1E40[0x4550]; int x6390; } Rider;
typedef struct State { Rider *rider; char pad4[0x10]; int action; unsigned char ready; unsigned char active; unsigned char notified; char pad1B[0x11]; int x2c; } State;
typedef struct Packed16 { char data[16]; } __attribute__((packed)) Packed16;
extern int func_00204230(void *);
static inline int A(Rider *r) { int value; if(r->x13DC) value=r->x126C; else value=r->x11A4; return value; }
static inline int B(Rider *r) { int value; if(r->x13DB) value=r->x1268; else value=r->x11A0; return value; }
static inline int C(Rider *r) { int value; if(r->x13DD) value=r->x1270; else value=r->x11A8; return value; }
static inline Packed16 Get(char *base,int index) { Packed16 *p=(Packed16 *)(base+(index*16+0x14)); return *p; }
int func_00153AA8(State *state) {
    Packed16 first,second;
    int action=0;
    Rider *r=state->rider;
    char *base=r->x1E40;
    if(state->x2c!=-1) return 0;
    if(r->x6390!=4) { state->active=0; goto finish; } else {
    if(C(r)) { state->active=0; action=0; goto finish; }
    if(!state->ready && !A(r) && !B(r)) state->ready=1;
    if(!state->action) {
        if(state->ready!=0) goto choose;
        if(state->active!=0) {
choose: {
            int value=A(r);
            action=0xB8;
            if(!value) { value=B(r); if(value==0) action=0; } }
        }
    } else {
        first=Get(base,state->action);
        if(func_00204230(&first)) {
            action=state->action;
            second=Get(base,action);
            if(func_00203C28(&second)>0.35f) {
                if(!state->notified) state->notified=1;
            }
        }
    }
}
finish:
    return action;
}
typedef struct Matrix { float m[16]; } Matrix __attribute__((aligned(16)));
typedef struct Item { unsigned int *x0; char pad4[12]; u128 x10; char pad20[32]; Matrix x40; } Item;
typedef struct Owner { char pad0[0x7ADC]; int x7ADC; } Owner;
typedef struct DropState { Owner *x0; char pad4[8]; float xC; int x10,x14; char pad18[16]; Item *x28; int x2C; } DropState;
extern void func_001525E8(DropState *);
extern Item *func_001591D8(unsigned int *);



extern void func_001D06C0(void *, int);
void func_00153C70(DropState *state) {
 char *part; char *part2; Owner *obj; int index, frame;
 func_001525E8(state);
 part=(char *)state->x0+0x1E40;
 obj=state->x0;
 obj->x7ADC--;
 state->x2C=-1;

 if(state->x14) {
  if(D_0029C658) {
   state->x28=func_001591D8(D_0029C658);
   *state->x28->x0=(*state->x28->x0 & 0x3FFFF) | ((*D_0029C658 >> 18)<<18);
   index=func_001B1C08(part);
   COPY_VECTOR(state->x28->x10,&D_007685D0[index]);
   __asm__ volatile("lq $8,0(%4)\n\tlq $9,16(%4)\n\tlq $10,32(%4)\n\tlq $11,48(%4)\n\tsq $8,%0\n\tsq $9,%1\n\tsq $10,%2\n\tsq $11,%3" : "=m"(((u128 *)&state->x28->x40)[0]), "=m"(((u128 *)&state->x28->x40)[1]), "=m"(((u128 *)&state->x28->x40)[2]), "=m"(((u128 *)&state->x28->x40)[3]) : "r"(&D_00769CB0[index]) : "$8", "$9", "$10", "$11");
  }
  if(((unsigned char *)part)[0x2160]) {
   part2=(char *)obj+0x3530;
   frame=(int)state->xC;
   func_001B1698((RiderBlendController *)((char *)obj+0x3AF0), frame, 0.0f);
   SetSecondaryBlendWeightFirst((RiderSecondaryBlend *)(part2), 0.25f, frame);
   SetBodyBlendWeightFirst((RiderBodyBlend *)((char *)obj+0x47F0), 0.25f, frame);
  }
  func_001D06C0((char *)obj+0x3FA8,4);
 }
}

void func_00153DD0(struct Obj *obj,int arg) {
 struct Mat m0,m1;
 struct Vec angles;
 struct Packed vec;
 struct Vec offset;
 struct Parent *parent=obj->x0;
 char *sub=(char *)parent+0x1e40;
 int index;
 if(D_0029C658 && obj->x2c==-1) {
 index=func_001B1C08(sub);
 COPY_VECTOR(obj->x28->pos,&D_007685D0[index]);
 COPY_MATRIX(obj->x28->mat,&D_00769CB0[index]);
 angles.x=D_0029C65C; angles.y=D_0029C660; angles.z=D_0029C664;
 func_00222040(&m0,&angles);
 func_00221930(&m1,&m0,&obj->x28->mat);
 COPY_MATRIX(obj->x28->mat,&m1);
 { unsigned int offset = obj->x14*16+20; memcpy(&vec,sub+offset,sizeof(vec)); }
 if(func_00203C28(&vec)>0.4f) {
 COPY_VECTOR(obj->x28->pos,&parent->x792c->pos);
 offset.x=D_0029C668; offset.y=D_0029C66C; offset.z=D_0029C670;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(vec):"r"(&offset),"r"(&parent->x792c->mat));
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(obj->x28->pos):"r"(&obj->x28->pos),"r"(&vec));
 func_00154A70(obj); func_00154B90(obj); obj->x2c=0; obj->x90=func_00238F48();
 }
 }
 func_001524D8(obj,arg);
}
