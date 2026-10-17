/* UNIT 0x00239A80 0x0023AE58 .sdata=0x29DC6C:0x29DC9C */
#include "scalar_api.h"
#include "include_asm.h"
typedef struct ClothVec { float x,y,z,w; } ClothVec __attribute__((aligned(16)));
typedef struct ClothPoint { char pad0[16]; ClothVec pos; char pad20[48]; } ClothPoint;
typedef struct ClothLink { signed char p[5]; unsigned char reverse; } ClothLink;
typedef struct ClothObject { ClothVec pos; char pad10[0x38]; signed char links[1]; } ClothObject;

int D_0029DC6C = 5;
float D_0029DC70 = 15.0f;
float D_0029DC74 = 0.99f;
float D_0029DC78 = 0.005f;
float D_0029DC7C = 0.005f;
float D_0029DC80 = 0.02f;
float D_0029DC84 = 0.75f;
float D_0029DC88 = 0.0f;
int D_0029DC8C = 1;
int D_0029DC90 = 1;
float D_0029DC94 = 0.2f;
unsigned int D_0029DC98 = 0xc0000080;
void func_00239A80(float a, float b, float c, float d) {
    D_0029DC78 = a;
    D_0029DC7C = b;
    D_0029DC80 = c;
    D_0029DC84 = d;
}

extern float D_0029DC88;

void func_00239A98(float v) {
    D_0029DC88 = v;
}

typedef struct { float x, y, z, w; } NVec __attribute__((aligned(16)));
typedef struct { NVec vec; float x10; short x14; short x16; short x18; } ESub;

static inline void NSub(NVec *dst, NVec *a, NVec *b) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline void NCross(NVec *dst, NVec *a, NVec *b) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline float NDot(NVec *a, NVec *b) {
    float d;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(d) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return d;
}
void func_00239AA0(ESub *sub, void *tablep, int mode) {
    NVec *table = (NVec *)tablep;
    NVec d1, d2;
    float x, y, z, len, root, inv;
    int bits;
    union { int bits; float f; } one;
    if (sub->x18 < 0) return;
    NSub(&d1, &table[sub->x16], &table[sub->x14]);
    NSub(&d2, &table[sub->x18], &table[sub->x14]);
    if (mode) NCross(&sub->vec, &d1, &d2);
    else NCross(&sub->vec, &d2, &d1);
    x = sub->vec.x;
    y = sub->vec.y;
    z = sub->vec.z;
    __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1"
        : "=&f"(root),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z) : "$f0");
    __asm__ volatile("" : : : "$3");
    one.bits = 0x3f800000;
    __asm__ volatile("" : : "r"(one.bits));
    inv = one.f; inv = inv / len;
    __asm__ volatile("" : : "f"(len),"f"(inv),"r"(one.bits));
    __asm__ volatile("" : : : "memory");
    sub->vec.x = x * inv;
    sub->vec.y = y * inv;
    sub->vec.z = z * inv;
    sub->x10 = NDot(&sub->vec, &table[sub->x14]);
}

__asm__(".space 8");

typedef struct { float x,y,z,w; } BVec __attribute__((aligned(16)));
typedef struct { BVec vec; char pad10[0x40]; } BEntry;
typedef struct { int count; char pad4[0x2c]; BEntry entries[19]; char pad620[0x30]; BVec x650; float x660; char pad664[0x719]; unsigned char xD7D; } BObj;
#define SCALE(out,in,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(out) : "r"(in),"f"(s) : "$2")
#define COPY(out,in) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(out) : "r"(in) : "$2")
void func_00239BA8(BObj *obj) {
 BVec base,large,medium; register int i; BVec *dst;
 SCALE(base,&obj->x650,obj->x660);
 if(obj->xD7D) { SCALE(large,&obj->x650,obj->x660+obj->x660); SCALE(medium,&obj->x650,obj->x660*1.5f); }
 for(i=0;i<obj->count;i++) {
  if(obj->xD7D) { if(i==0 || i==3 || i==6 || i==9 || i==12) { COPY(obj->entries[i].vec,&large); }
  else if(i==2 || i==5 || i==8 || i==11) { COPY(obj->entries[i].vec,&medium); }
  else { COPY(obj->entries[i].vec,&base); } } else { COPY(obj->entries[i].vec,&base); }
 }
}

typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Particle {
    Vec4 pos;
    Vec4 prev;
    Vec4 force;
    char pad30[0x20];
} Particle;

typedef struct System {
    int count;
    char pad4[0xC];
    Particle particles[1];
} System;


/* as sceVu0CopyVector */
static inline void CopyVector(Vec4 *dst, Vec4 *src)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
}

/* Verlet step: pos += (pos - prev) * damping + force * dt^2 (dt = 1/60). */
void func_00239CE0(System *sys)
{
    Particle *p;
    Vec4 old;
    int i;
    float dt2;

    dt2 = 0.0002777778f;
    for (i = 0; i < sys->count; i++) {
        p = &sys->particles[i];
        CopyVector(&old, &p->pos);
        p->pos.x += (p->pos.x - p->prev.x) * D_0029DC74 + p->force.x * dt2;
        p->pos.y += (p->pos.y - p->prev.y) * D_0029DC74 + p->force.y * dt2;
        p->pos.z += (p->pos.z - p->prev.z) * D_0029DC74 + p->force.z * dt2;
        CopyVector(&p->prev, &old);
    }
}

typedef struct { char pad0[0x60]; BVec x60; char pad70[0x688 - 0x70]; short x688; char pad68a[0xD7D - 0x68A]; unsigned char xD7D; } GObj;

extern char D_007684B0[];
void func_00239E70();

void func_00239DA0(void *obj, int count)
{
    GObj *g = (GObj *)obj;
    int n;
    int i;

    if (!D_0029DC90) return;
    func_00239E40(obj);
    if (g->x688 >= 0 && !g->xD7D) {
        COPY(g->x60, (BVec *)D_007684B0 + g->x688);
    }
    n = g->xD7D ? D_0029DC6C : count;
    for (i = 0; i < n; i++) func_00239E70(obj);
}

void func_00239E40(void *obj) {
    func_00239BA8((BObj *)obj);
    func_00239CE0((System *)obj);
}

typedef struct EItem { char pad0[0x50]; } EItem;
/* ESub is defined above, next to func_00239AA0. */

typedef struct EObj {
    int count;
    char pad4[0xC];
    EItem items[(0x664 - 0x10) / 0x50];
    char pad654[0x664 - 0x10 - ((0x664 - 0x10) / 0x50) * 0x50];
    signed char subCount;
    char pad665[0x670 - 0x665];
    ESub subs[(0xD7C - 0x670) / 0x20];
    char pad[0xD7C - 0x670 - ((0xD7C - 0x670) / 0x20) * 0x20];
    unsigned char xD7C;
    unsigned char xD7D;
} EObj;

extern char D_007684B0[];
extern int D_0029DC8C;
extern void func_00239AA0(ESub *sub, void *table, int mode);
extern void func_0023A0D8();
extern void func_0023A278();
extern void func_0023A6E0(ClothPoint *points, ClothObject *o);
extern void func_0023A450(EObj *obj, EItem *item);
extern void func_00239F78();
extern void func_0023A250();

void func_00239E70(EObj *obj)
{
    int i;

    if (!obj->xD7D) {
        for (i = 0; i < obj->subCount; i++) {
            func_00239AA0(&obj->subs[i], D_007684B0, obj->xD7C);
        }
    }
    for (i = 0; i < obj->count; i++) {
        func_0023A0D8(obj, &obj->items[i]);
        if (obj->xD7D && D_0029DC8C == 1) {
            func_0023A278(obj, &obj->items[i]);
            func_0023A6E0((ClothPoint *)obj, (ClothObject *)&obj->items[i]);
            func_0023A450(obj, &obj->items[i]);
        }
        func_00239F78(obj, &obj->items[i]);
        func_0023A250(obj, &obj->items[i]);
    }
}


typedef struct Plane {
    float normal[4];
    float dist;
    short point;
    char pad16[4];
    unsigned char x1a;
    unsigned char x1b;
    unsigned char dirty;
    char pad1d[3];
} Plane;

typedef struct Point {
    char pad0[0x10];
    float pos[4];
    char pad20[0x30];
} Point;

typedef struct World {
    Point points[20];
    char pad640[0x30];
    Plane planes[48];
    char padC70[0x10D];
    unsigned char xD7D;
} World;

typedef struct PObj {
    float pos[4];
    char pad10[0x32];
    signed char planes[6];
} PObj;

static inline float Dot(float *a, float *b)
{
    float out;
    __asm__ volatile ("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6"
             : "=f"(out)
             : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    return out;
}

void func_00239F78(World *world, PObj *obj)
{
    int i;
    int idx;
    Plane *plane;
    float dist;
    float scale;

    for (i = 0; i < 6; i++) {
        idx = obj->planes[i];
        if (idx < 0) {
            break;
        }
        plane = &world->planes[idx];
        if (plane->dirty) {
            plane->dist = Dot(plane->normal, world->points[plane->point].pos) + 0.1f;
        }
        dist = Dot(plane->normal, obj->pos) - plane->dist;
        if (plane->x1a && dist >= 0.0f) {
            continue;
        }
        if (world->xD7D && !plane->x1b) {
            if (dist < -D_0029DC80) {
                dist = -D_0029DC80;
            } else if (dist > D_0029DC80) {
                dist = D_0029DC80;
            }
        }
        scale = -(dist * 0.99f);
        __asm__ volatile ("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tmfc1 $2,%2
	qmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,0(%0)"
                          : : "r"(obj), "r"(plane), "f"(scale) : "$2", "memory");
    }
}

typedef struct { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct { Vec pos; char pad10[0x24]; int x34; signed char links[6]; } Node;
typedef struct { short index, type; float length; } Link;
extern float D_0029DC84;
#define SUB(out,a,b) __asm__("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out) : "r"(a),"r"(b) : "memory")
#define ADD(out,a,b) __asm__("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out) : "r"(a),"r"(b) : "memory")
#define SCALE(out,in,s) __asm__("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(out) : "r"(in),"f"(s) : "$2")
static inline float Ratio(Vec *v, float len) {
    float r;
    __asm__ volatile("lwc1 $f1,0(%2)\n\tlwc1 $f2,4(%2)\n\tmula.s $f1,$f1\n\tmadda.s $f2,$f2\n\tlwc1 $f1,8(%2)\n\tmadd.s %1,$f1,$f1\n\trsqrt.s %1,%0,%1"
                     : "+f"(len), "=&f"(r) : "r"(v) : "$f1", "$f2");
    return r;
}
void func_0023A0D8(char *obj, Node *node) {
    Vec delta;
    int i, index;
    Link *link;
    Node *other;
    float f, scale;
    for (i = 0; i < 6 && (index = node->links[i]) >= 0; i++) {
        link = (Link *)(obj + 0xbac + index * 8);
        other = (Node *)(obj + 0x10 + link->index * 0x50);
        SUB(delta, &other->pos, &node->pos);
        f = 1.0f - Ratio(&delta, link->length);
        if (link->type == 0) {
            if (other->x34 >= 0) {
                SCALE(delta, &delta, f);
                ADD(node->pos, &node->pos, &delta);
                continue;
            }
            if (node->x34 >= 0) {
                SCALE(delta, &delta, f);
                goto move_other;
            }
            scale = f * 0.5f;
        } else if (link->type == 2) {
            if (!(f > 0.0f)) continue;
            scale = f * (D_0029DC84 * 0.5f);
        } else if (link->type == 1) {
            if (!(f < 0.0f)) continue;
            scale = f * (D_0029DC84 * 0.5f);
        } else {
            continue;
        }
        SCALE(delta, &delta, scale);
        ADD(node->pos, &node->pos, &delta);
    move_other:
        SUB(other->pos, &other->pos, &delta);
    }
}

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef struct Vec4B {
    float x, y, z, w;
} Vec4B __attribute__((aligned(16)));

typedef struct EItemB {
    Vec4B x0;
    char pad10[0x24];
    int x34;
} EItemB;


void func_0023A250(void *obj, EItemB *item) {
    int index = item->x34;
    if (index >= 0) {
        COPY_VECTOR(item->x0, &((Vec4B *)D_007684B0)[index]);
    }
}

typedef struct VecC { float v[4]; } VecC __attribute__((aligned(16)));
typedef struct ObjC { VecC x0,x10; char pad20[0x29]; signed char ids[1]; } ObjC;
static inline void VecSub(VecC *dst,void *a,void *b){
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0":"=m"(*dst):"r"(a),"r"(b));
}
static inline void VecAdd(VecC *dst,void *a,void *b){
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0":"=m"(*dst):"r"(a),"r"(b));
}
static inline void Normalize(VecC *dst,VecC *src) {
 float inv,len;
 __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(len):"f"(src->v[0]),"f"(src->v[1]),"f"(src->v[2]),"f"(src->v[2]):"$2");
 dst->v[0]=src->v[0]*inv;dst->v[1]=src->v[1]*inv;dst->v[2]=src->v[2]*inv;
}
static inline void NormalizeB(VecC *dst,VecC *src) {
 float inv,len; float x=src->v[0],y=src->v[1],z=src->v[2];
 __asm__ volatile("mula.s %2,%3\n\tmadda.s %4,%5\n\tmadd.s %1,%6,%7\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(len):"f"(x),"f"(x),"f"(y),"f"(y),"f"(z),"f"(z):"$2");
 dst->v[0]=x*inv;dst->v[1]=y*inv;dst->v[2]=z*inv;
}
static inline void NormalizeA(VecC *dst,VecC *src) {
 float inv,len;
 __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(len):"f"(src->v[0]),"f"(src->v[1]),"f"(src->v[2]):"$2");
 dst->v[0]=src->v[0]*inv;dst->v[1]=src->v[1]*inv;dst->v[2]=src->v[2]*inv;
}
void func_0023A278(char *base,ObjC *o){
 VecC a,b,na,nb,c,d,nd; signed char *ids; int count=0,index,id; float dot,scale;
 if((id=o->ids[0])!=-1) { do {
 index=*(int *)(base+0xD3C+id*8);
 VecSub(&a,&o->x10,base+index*80+0x20);
 VecSub(&b,&o->x0,base+index*80+0x10);
 NormalizeA(&na,&a);NormalizeB(&nb,&b);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(dot):"f"(na.v[0]),"f"(nb.v[0]),"f"(na.v[1]),"f"(nb.v[1]),"f"(na.v[2]),"f"(nb.v[2]));
 if(dot<D_0029DC88){
 VecAdd(&c,base+index*80+0x10,&a);scale=D_0029DC88-dot;
 VecSub(&d,&c,&o->x0);Normalize(&nd,&d);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0":"=m"(nd):"r"(&nd),"f"(scale):"$2");
 VecAdd(&o->x0,&o->x0,&nd);
 }
 count++;if(count>0)break;
 }while((id=o->ids[count])!=-1); }
}

typedef float Vector[4] __attribute__((aligned(16)));
typedef struct {char pad0[0x10];Vector pos;char pad20[0x30];} CollisionPoint;
typedef struct {signed char indices[4];float radius;} Edge;
typedef struct {CollisionPoint points[1];} CollisionWorld;
typedef struct {Vector pos;char pad10[0x2e];signed char edges[4];} Body;

static inline void add(Vector dst,Vector a,Vector b) {
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0":"=m"(*dst):"r"(a),"r"(b):"memory");
}
static inline void sub(Vector dst,Vector a,Vector b) {
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0":"=m"(*dst):"r"(a),"r"(b):"memory");
}
static inline void scale(Vector dst,Vector src,float value) {
 __asm__ volatile("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0":"=m"(*dst):"r"(src),"f"(value):"$2","memory");
}
static inline float dot(Vector a,Vector b) {float result;
 __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6":"=f"(result):"f"(a[0]),"f"(b[0]),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2]));return result;
}
static inline float root(float value) {float result;__asm__ volatile("sqrt.s %0, %1":"=f"(result):"0"(value));return result;}
static inline float magnitude(Vector v) {
 float result;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0":"=&f"(result):"f"(v[0]),"f"(v[1]),"f"(v[2]));
 return result;
}
static inline void normalize(Vector dst,Vector src) {
 float x=src[0],y=src[1],z=src[2],length,inv;int bits;
 /* Sony length/reciprocal wrapper, as in 0010ED50. */
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(length),"=r"(bits):"f"(x),"f"(y),"f"(z));
 dst[0]=x*inv;dst[1]=y*inv;dst[2]=z*inv;
}
static inline float fraction(Vector a,Vector b) {float numerator=a[0],denominator=b[0];
 __asm__("mula.s %2, %3
	madda.s %4, %5
	madd.s %0, %6, %7
	mula.s %3, %3
	madda.s %5, %5
	madd.s %1, %7, %7":"=f"(numerator),"=f"(denominator):"0"(numerator),"1"(denominator),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2]));return numerator/denominator;
}
void func_0023A450(EObj *worldArg,EItem *bodyArg) {
 CollisionWorld *world=(CollisionWorld *)worldArg;Body *body=(Body *)bodyArg;
 Vector second,first,toBody,direction,scaled,closest,delta,normal,push;
 int i,index; signed char *edgeIndex;float *radius;Edge *edge;float factor,length,depth,inv;
 index=body->edges[0];i=0;if(index!=-1) {do {radius=(float *)((char *)world+0xb74+i*8);
 edge=(Edge *)((char *)world+0xb70+index*8);
 add(first,world->points[edge->indices[0]].pos,world->points[edge->indices[1]].pos);
 add(second,world->points[edge->indices[2]].pos,world->points[edge->indices[3]].pos);
 scale(first,first,0.5f);scale(second,second,0.5f);
 sub(direction,second,first);sub(toBody,body->pos,first);
 factor=dot(toBody,direction);length=dot(direction,direction);factor/=length;
 if(!(factor < -0.3f) && !(factor > 1.3f)){
 scale(scaled,direction,factor);add(closest,first,scaled);sub(delta,closest,body->pos);
 length=magnitude(delta);depth=*radius-length;
 if(depth>0.0f){
 sub(normal,body->pos,closest);normalize(delta,normal);
 if(depth>D_0029DC78)depth=D_0029DC78;
 scale(push,delta,depth);add(body->pos,body->pos,push);
 }
 }
 i++;
 if(i>=4)break;index=body->edges[i];
 }while(index!=-1); }
}

static inline void ClothSub(ClothVec *dst, void *a, void *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*dst):"r"(a),"r"(b)); }
#define CLOTH_CROSS(out,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0":"=m"(out):"r"(a),"r"(b))
#define CLOTH_ADD_SCALED(out,a,b,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(out):"r"(a),"r"(b),"f"(s):"$2","memory")
static inline void ClothAddScaled(ClothVec *out, void *a, void *b, float s) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"r"(b),"f"(s):"$2","memory");
}
static inline void ClothNormalize(ClothVec *dst, ClothVec *src) {
 float x=src->x,y=src->y,z=src->z,len,inv;
 int bits;
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1":"=&f"(inv),"=&f"(len),"=r"(bits):"f"(x),"f"(y),"f"(z));
 dst->x=x*inv; dst->y=y*inv; dst->z=z*inv;
}
static inline float ClothDot(ClothVec *a,ClothVec *b) {
 float out;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(out):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return out;
}

void func_0023A6E0(ClothPoint *points,ClothObject *o) {
 ClothVec a,b,c,normal,other,d,e;
 int count=0,id;
 ClothLink *link;
 char *base=(char *)points;
 float dot;
 if((id=o->links[0])!=-1) { do {
  link=(ClothLink *)(base+0xba0+id*6);
  ClothSub(&a,&points[link->p[0]].pos,&points[link->p[1]].pos);
  ClothSub(&b,&points[link->p[2]].pos,&points[link->p[1]].pos);
  if(link->reverse) { CLOTH_CROSS(c,&a,&b); } else { CLOTH_CROSS(c,&b,&a); }
  ClothNormalize(&normal,&c);
  ClothSub(&a,&points[link->p[3]].pos,&o->pos);
  ClothSub(&b,&points[link->p[4]].pos,&o->pos);
  if(link->reverse) { CLOTH_CROSS(c,&a,&b); } else { CLOTH_CROSS(c,&b,&a); }
  ClothNormalize(&other,&c);
  dot=ClothDot(&normal,&other);
  if(dot<0.05f) {
   float firstScale;
   firstScale=D_0029DC7C*-dot;
   ClothSub(&a,&points[link->p[1]].pos,&points[link->p[0]].pos);
   ClothNormalize(&d,&a);
   ClothAddScaled(&o->pos,&o->pos,&d,firstScale);
   ClothAddScaled(&points[link->p[4]].pos,&points[link->p[4]].pos,&d,-firstScale);
   ClothAddScaled(&points[link->p[1]].pos,&points[link->p[1]].pos,&normal,firstScale*D_0029DC94);
   ClothAddScaled(&points[link->p[2]].pos,&points[link->p[2]].pos,&normal,firstScale*D_0029DC94);
   ClothAddScaled(&points[link->p[0]].pos,&points[link->p[0]].pos,&normal,-firstScale*D_0029DC94);
  }
  ClothSub(&a,&points[link->p[1]].pos,&points[link->p[0]].pos);
  ClothNormalize(&d,&a);
  dot=ClothDot(&d,&other);
  if(dot>-0.3f) {
   float secondScale;
   secondScale=D_0029DC7C;
   ClothSub(&a,&points[link->p[2]].pos,&points[link->p[1]].pos);
   ClothSub(&b,&points[link->p[4]].pos,&points[link->p[3]].pos);
   ClothNormalize(&d,&a); ClothNormalize(&e,&b);
   dot=ClothDot(&d,&e);
   if(dot<0.0f) secondScale*= -dot; else secondScale*=dot;
   ClothAddScaled(&o->pos,&o->pos,&normal,secondScale);
   if(dot>0.5f) {
    ClothAddScaled(&points[link->p[1]].pos,&points[link->p[1]].pos,&normal,-secondScale*D_0029DC94);
    ClothAddScaled(&points[link->p[2]].pos,&points[link->p[2]].pos,&normal,-secondScale*D_0029DC94);
    ClothAddScaled(&points[link->p[0]].pos,&points[link->p[0]].pos,&normal,secondScale*D_0029DC94);
   }
   ClothSub(&a,&points[link->p[1]].pos,&points[link->p[2]].pos);
   ClothSub(&b,&points[link->p[4]].pos,&points[link->p[3]].pos);
   ClothNormalize(&d,&a); ClothNormalize(&e,&b);
   dot=ClothDot(&normal,&e);
   secondScale=D_0029DC7C*dot;
   ClothAddScaled(&o->pos,&o->pos,&d,secondScale);
  }
  count++; if(count>0) break;
 } while((id=o->links[count])!=-1); }
}
