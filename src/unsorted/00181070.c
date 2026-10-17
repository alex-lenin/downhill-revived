/* UNIT 0x00181070 0x00181228 */
#include "spatial_primitives.h"
#if defined(__mips__)
#define CallShared00211BE0_0 func_00211BE0
#else
#define CallShared00211BE0_0 Forward00211BE0_2
#endif
#if defined(__mips__)
#define CallShared00211BE0_1 func_00211BE0
#else
#define CallShared00211BE0_1 Forward00211BE0_2
#endif
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj { char pad[0x190]; Vec plane; char pad1a0[0x18]; signed char x1b8; } Obj;
typedef struct Hit { unsigned short flag; char pad2[0xe]; Vec pos,normal; } Hit;
typedef struct Filter { unsigned int x0; unsigned short x4; unsigned short x6; } Filter;
extern float D_0029CAB4[],D_0029CAB8[];
extern Filter *func_00210940(Filter *);
extern int func_00211BE0(Vec *,float,Hit *,float);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static __inline__ void init(Filter *f) { f->x0=0xffffffff; f->x4=0xffff; }
static __inline__ void mask(Filter *f,unsigned m) { *(unsigned *)((char *)f+4)&=m; }
static __inline__ void set(Filter *f,unsigned m) { *(unsigned *)((char *)f+4)|=m; }
static __inline__ void reset(Vec *plane,float x,float y,float z,float w) {
    plane->x=x;
    plane->y=y;
    plane->z=z;
    plane->w=w;
}
static __inline__ float plane_height(Vec *plane,Vec *pos) {
    if(plane->z!=0.0f) return ((plane->x*pos->x+plane->y*pos->y)-plane->w)/-plane->z;
    return D_0029CAB8[0];
}
static __inline__ Filter *query(Vec *pos,Hit *hit,Filter *old) { CallShared00211BE0_0(pos,16.0f,hit,-48.0f); return old; }
float func_00181070(Obj *obj,Vec *pos,int force) {
    Hit hit;
    Filter filter;
    unsigned int old;
    float height=D_0029CAB4[0];
    float dot,y;
    if(force || obj->x1b8) {
        init(&filter);
        filter.x0=0;
        mask(&filter,0xfffeffff);
        mask(&filter,0xfffdffff);
        mask(&filter,0xfffbffff);
        mask(&filter,0xfffeffff);
        set(&filter,0x10000);
        {
            Hit *result=&hit;
            float upper=16.0f,lower=-48.0f;
            old=(unsigned int)func_00210940(&filter);
            CallShared00211BE0_1(pos,upper,result,lower);
        }
        func_00210940((Filter *)old);
        if(hit.flag) {
            COPY_VECTOR(obj->plane,&hit.normal);
            __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(hit.normal.x),"f"(hit.pos.x),"f"(hit.normal.y),"f"(hit.pos.y),"f"(hit.normal.z),"f"(hit.pos.z));
            obj->plane.w=dot;
            height=pos->z-hit.pos.z;
        }
        if(height>48.0f) {
            reset(&obj->plane,0.0f,0.0f,1.0f,D_0029CAB8[0]);
        }
    } else {
        y=pos->z;
        height=y-plane_height(&obj->plane,pos);
    }
    return height;
}
