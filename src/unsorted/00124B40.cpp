/* UNIT 0x00124B40 0x00124CE8 */
#include "rider_ai.h"
#include "rider_geometry.h"
#include "animation.h"
struct Vec { float x,y,z,w; };
typedef struct __attribute__((packed)) { float x,y,z,w; } PackedVec;
typedef struct { char pad[0x20]; short delta; short pad22; void *(*call)(void *, void *); } Vtable;
struct Model { virtual void unused1(); virtual void unused2(); virtual void unused3(); virtual void *get(); };
typedef struct { char pad0[0x4180]; Vec x4180; char pad4190[0x379c]; char *x792c; } Rider;
typedef struct { char pad0[8]; int x8; char padc[0xc]; int x18; char pad1c[0x24]; int x40[60]; Model *x130; char pad134[0x10]; Rider *x144; char pad148[0x98]; Vec x1e0, x1f0; } Obj;
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
extern "C" {
void func_001258A0(Obj *, float);

float *func_00261718(Vec *), *func_00261710(Vec *), *func_00261028(Vec *);

float func_00238F40(void);
void func_00236558(Vec *,PackedVec *,float);
void func_00123DB0(Obj *,void *), func_00124300(Obj *), func_00211280(void *), func_00127098(Obj *);
}
static inline void setRadius(Vec *v, float f) { *func_00261718(v) = f; }
static inline void setSpeed(Vec *v, float f) { *func_00261710(v) = f; }
void func_00124B40(void *view) {
    Obj *obj=(Obj *)view;
    PackedVec pos;
    PackedVec *src;
    Vec first __attribute__((aligned(16))), second __attribute__((aligned(16)));
    
    unsigned int active=1;
    if(obj->x144 == 0) goto finish;
    {
        Vec *p;
        float value; float saved;
        Model *m;
        Vtable *vt;
        func_001258A0(obj, 29.3333339691162109375f);
        { src=(PackedVec *)((char *)obj->x130 + (obj->x40[obj->x8]*16 + 16)); pos=*src; func_00203E10((const AnimationHandle *)&pos,1.0f); }
        saved=first.w;
        COPY_VECTOR(first,obj->x144->x792c+16);
        first.w=saved;
        saved=second.w;
        COPY_VECTOR(second,&obj->x144->x4180);
        second.w=saved;
        setRadius(&first,0.25f);
        p=&obj->x1e0;
        value=2.0f * *func_00261028(p);
        setSpeed(&first,value);
        func_002362E8((float *)(&first), (Vector4 *)(p), (Vector4 *)(&obj->x1f0), (Vector4 *)(&pos));
        func_00236558(p,&pos,func_00238F40());
        m=obj->x130;
        func_00123DB0(obj,(char *)m->get()+0x40);
        func_00124300(obj);
        m=obj->x130;
        func_00211280(m->get());
        if (obj->x18<=179) active=0;
    }
finish:
    if (active) func_00127098(obj);
}
