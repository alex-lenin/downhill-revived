/* UNIT 0x001B08D0 0x001B0A48 */
#include "spatial.h"
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Node { char pad[0x40]; Vec x40,x50,x60; } Node;
typedef struct Origin { char pad[0x10]; Vec pos; } Origin;
typedef struct State { char pad[0x16d4]; Origin *x16d4; } State;
typedef struct Parent { char pad[0x1e40]; State state; char pad3518[0x4414]; Node *x792c; } Parent;
typedef struct Obj { Vec pos; Parent *x10; char pad14[4]; float x18; } Obj;
typedef struct Hit { unsigned short flag; char pad2[14]; Vec pos; char pad20[16]; } Hit;

#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define SCALE(dst,src,s) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(src), "f"(s) : "$2")
#define ADD(dst,a,b) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a),"r"(b))
static __inline__ void normalize(Vec *v) {
    float length,scale;
    __asm__ volatile("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	"
                     "lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1"
                     : "=&f"(scale), "=&f"(length) : "f"(v->x),"f"(v->y),"f"(v->z) : "$2");
    v->x*=scale;
    v->y*=scale;
    v->z*=scale;
}
static __inline__ void query(Vec *a,Vec *b,Hit *hit) { func_00214A18((Vector4 *)(a), (Vector4 *)(b), (SpatialHit *)(hit), 1, 0.1f, 0.0f, 1.0f); }
int func_001B08D0(Obj *obj) {
    Vec direction,offset;
    Hit hit;
    Parent *parent=obj->x10;
    Node *node=parent->x792c;
    State *state=&parent->state;
    { Vec *out=&offset;
    SCALE(*out,&node->x60,-0.8f);
    SCALE(direction,&node->x50,0.5f);
    ADD(offset,&offset,&direction);
    SCALE(direction,&node->x40,obj->x18*2.0f);
    ADD(direction,&direction,out);
    }
    normalize(&direction);
    SCALE(direction,&direction,3.0f);
    ADD(obj->pos,&state->x16d4->pos,&direction);
    query(&state->x16d4->pos,&obj->pos,&hit);
    if(hit.flag) COPY_VECTOR(obj->pos,&hit.pos);
    return hit.flag!=0;
}
