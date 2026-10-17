/* UNIT 0x00135A30 0x00136030 .sdata=0x29C2FC:0x29C310 */
#include "object_registry.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Flag { unsigned long x0; };
struct Contact { unsigned short x0,x2,x4,x6; int x8; Flag *xC; Vec x10,x20; };
struct Body { char pad0[0x30]; Contact x30; char pad60[0x14]; unsigned char x74; int state() { return x74; } char pad75[0xB]; };
struct Parent {
 char pad0[0x11D4]; int x11D4; char pad11D8[0xC4]; int x129C; char pad12A0[0x148]; unsigned char x13E8;
 char pad13E9[0x655B]; void *x7944,*x7948; char pad794C[0x30]; int x797C; char pad7980[0x21E]; unsigned char x7B9E,x7B9F; char pad7BA0[0x11]; unsigned char x7BB1;
};
struct Obj {
 char pad0[0x38]; Vec *x38; char pad3C[0x84]; Vec xC0; char padD0[0x88]; Parent *x158; char pad15C[0xA4];
 Contact x200; char pad230[0x70]; Vec x2A0,x2B0,x2C0; char pad2D0[0x10]; Body x2E0,x360; char pad3E0[0x50];
 int x430,x434; char pad438[0x54]; float x48C; int x490,x494; char pad498[0xC]; short x4A4;
};
struct VirtualObj { virtual void a(); virtual void b(); virtual void velocity(Vec *,Vec *); };
int D_0029C2FC=4;
float D_0029C300=0.45f;
int D_0029C304=1;
float D_0029C308=0.25f;
float D_0029C30C=1.3f;
extern void func_00169F08(void *);
extern int func_00138560(Obj *);
extern void func_00136038(Obj *),func_00131998(Obj *),func_00135478(Obj *),func_001385B0(Obj *);
extern void func_002093C8(Flag *,Vec *,Vec *);

extern void func_00132CE0(Obj *,Contact *,Vec *,int);
extern void func_001EA918(void *);
extern float func_00238F38();
extern void func_00134CC8(Body *,void *,float);
extern void func_0012DDC0(Parent *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline void zero(Vec *v) { __asm__ volatile("sqc2 $vf0,%0" : "=m"(*v)); }
static inline float dot(Vec *a,Vec *b) { float d; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(d) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return d; }
static inline float length2(Vec *a) { float d; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(d) : "f"(a->x),"f"(a->y),"f"(a->z)); return d; }
static inline float length(Vec *a) { float d; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(d) : "f"(a->x),"f"(a->y),"f"(a->z)); return d; }
static inline void addscale(Vec *d,Vec *a,Vec *b,float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b),"f"(f) : "$2"); }
static inline void cross(Vec *d,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b)); }
static inline void normalize(Vec *a) { float d,inv; float x=a->x,y=a->y,z=a->z; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(d) : "f"(x),"f"(y),"f"(z)); __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(d)); a->x=x*inv;a->y=y*inv;a->z=z*inv; }
/* velocity of the surface under contact c, into v */
#define CONTACT_VELOCITY(c, v) \
 if((c)->xC && ((c)->xC->x0 & ((unsigned long)1<<58))) func_002093C8((c)->xC,&(c)->x10,v); \
 else { VirtualObj *p=(VirtualObj *)func_0017D3D0((c)->x4); \
  if(p) { p->velocity(&(c)->x10,v); if((c)->x2==0x1E75) addscale(v,v,&(c)->x20,-dot(&(c)->x20,v)); } }
static inline void decay(Obj *o) {
    if (o->x48C >= 1.0f) o->x48C = 1.0f;
    else o->x48C *= D_0029C30C;
}
void func_00135A30(Obj *o) {
    Vec v, saved;
    void *frontArg, *backArg;
    float speed;

    o->x434 = 0;
    o->x430 = 0;
    o->x2E0.x74 = 2;
    ++o->x4A4;
    o->x360.x74 = 2;
    func_00169F08((char *)o + 0x160);

    /* Both branches test the same flag; gcc merges them after register allocation
       (cross-jumping), which leaves the x797C load without a use. */
    if (o->x158->x797C) {
        if (!((volatile Obj *)o)->x158->x7BB1) goto skip;
    } else {
        if (!((volatile Obj *)o)->x158->x7BB1) goto skip;
    }
    if (func_00138560(o)) {
        Parent *p = o->x158;
        int n;
        if (p->x13E8) n = p->x129C;
        else n = p->x11D4;
        if (n) o->x494 = 1;
        else o->x494 = 0;
    } else {
        o->x494 = 0;
    }
skip:
    func_00136038(o);
    func_00131998(o);

    if (D_0029C300 > 0.0f) {
        if (o->x2E0.state() == 1 || o->x360.state() == 1) {
            o->x4A4 = 0;
            zero(&v);
            if (o->x2E0.state() == 1) {
                Contact *c = &o->x2E0.x30;
                CONTACT_VELOCITY(c, &v)
                func_00132CE0(o, &o->x2E0.x30, &v, 0);
            }
            zero(&v);
            if (o->x360.state() == 1) {
                Contact *c = &o->x360.x30;
                CONTACT_VELOCITY(c, &v)
                func_00132CE0(o, &o->x360.x30, &v, 0);
            }
            decay(o);
        } else if (o->x360.state() == 2 && o->x2E0.state() == 2) {
            o->x48C = D_0029C308;
        } else if (o->x360.state() == 0 || o->x2E0.state() == 0) {
            decay(o);
        }
    } else if (o->x4A4 && !func_00138560(o)) {
        zero(&v);
        if (o->x4A4 < D_0029C2FC) func_00132CE0(o, &o->x200, &v, 0);
    }

    if (o->x4A4 == 1) {
        Vec *a = &o->x2A0, *b = &o->x2B0;
        Vec *c;
        v.x = 0;
        v.y = 0;
        c = &o->x2C0;
        v.z = 1;
        COPY_VECTOR(*b, &o->xC0);
        cross(a, b, &v);
        if (length2(a) < 0.01f) {
            COPY_VECTOR(*b, o->x38 + 1);
            cross(a, b, &v);
            if (length2(a) < 0.01f) {
                b->x = 0;
                b->y = 1;
                b->z = 0;
                cross(a, b, &v);
            }
        }
        cross(c, a, b);
        normalize(b);
        normalize(c);
        normalize(a);
        func_001EA918((char *)o->x158 + 0x5FF0);
    }

    func_00135478(o);
    frontArg = o->x158->x7944;
    backArg = o->x158->x7948;
    COPY_VECTOR(saved, &o->xC0);
    addscale(&v, &o->xC0, &o->x200.x20, -dot(&o->x200.x20, &o->xC0));
    speed = length(&v) / func_00238F38();
    o->x158->x7B9E = 0;
    o->x158->x7B9F = 0;
    func_001385B0(o);
    if (frontArg) func_00134CC8(&o->x2E0, frontArg, speed);
    if (backArg) func_00134CC8(&o->x360, backArg, speed);
    func_0012DDC0(o->x158);
}
