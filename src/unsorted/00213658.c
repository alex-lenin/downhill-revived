/* UNIT 0x00213658 0x00213C08 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: FP register choice and load order, Inside scratch stack slots, shared zero constant and delay-slot fill;
   best diff 121/363 words (with a tied asm operand, dropped here: 0x59C of 0x5AC bytes, four pointer copies fewer) */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
/* Sweep contact: pos.w = contact time, depth = penetration. 0x30 bytes. */
typedef struct Hit { Vec pos, normal; float depth; float pad[3]; } Hit;
extern float D_0029DA70[];
/* Swept sphere vs triangle edge tri[i]..tri[(i+1)%3] (src/unsorted/00213250.c; real order is
   origin, dir, bmin, bmax, segStart, segEnd, out, radius, tmax: floats travel in $f12/$f13 either way). */
extern int func_00213250(Vec *, Vec *, float *, float *, float, float, Vec *, Vec *, Hit *);
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
/* EE FPU multiply-accumulate dot product of xyz. */
static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}
static inline float Max(float a, float b) {
    float r;
    __asm__ volatile("max.s %0,%1,%2" : "=f"(r) : "f"(a), "f"(b));
    return r;
}
/* out.xyz = a.xyz + b.xyz * scale; out.w = a.w (VU0). */
static inline void AddScaled(Vec *out, Vec *a, Vec *b, float scale) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}
/* Triangle plane: xyz = normalize((tri[2]-tri[1]) x (tri[0]-tri[1])), w = dot(n, tri[1]). */
static inline void Plane(Vec *out, Vec *tri) {
    float x,y,z,len,inv,tx;
    __asm__ volatile("lqc2 $vf10,0(%1)\n\tlqc2 $vf12,16(%1)\n\tlqc2 $vf15,32(%1)\n\tvsub.xyz $vf14,$vf15,$vf12\n\tvsub.xyz $vf11,$vf10,$vf12\n\tvopmula.xyz ACC,$vf14,$vf11\n\tvopmsub.xyz $vf13,$vf11,$vf14\n\tsqc2 $vf13,0(%0)" : : "r"(out), "r"(tri) : "memory");
    x=out->x; y=out->y; z=out->z;
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    tx=tri[1].x;
    out->x=x*inv; out->y=y*inv; out->z=z*inv;
    { float r; Vec *point=tri+1;
    __asm__ volatile("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(r) : "f"(out->x),"f"(out->y),"f"(out->z),"f"(tx),"f"(point->y),"f"(point->z));
    out->w=r; }
}
/* Point-in-triangle: the three edge cross products all point the same way. */
static inline unsigned char Inside(Vec *pos, Vec *tri) {
    Vec bb,cc,aa;
    Vec *a=&aa,*b=&bb,*c=&cc;
    __asm__ volatile("lqc2 $vf10,0(%3)\n\tlqc2 $vf11,0(%4)\n\tlqc2 $vf12,16(%4)\n\tlqc2 $vf13,32(%4)\n\tvsub.xyz $vf14,$vf10,$vf11\n\tvsub.xyz $vf15,$vf10,$vf12\n\tvsub.xyz $vf16,$vf10,$vf13\n\tvopmula.xyz ACC,$vf14,$vf15\n\tvopmsub.xyz $vf11,$vf15,$vf14\n\tvopmula.xyz ACC,$vf15,$vf16\n\tvopmsub.xyz $vf12,$vf16,$vf15\n\tvopmula.xyz ACC,$vf16,$vf14\n\tvopmsub.xyz $vf13,$vf14,$vf16\n\tsqc2 $vf11,0(%0)\n\tsqc2 $vf12,0(%1)\n\tsqc2 $vf13,0(%2)" : : "r"(a), "r"(b), "r"(c), "r"(pos), "r"(tri) : "memory");
    if(Dot(a,b)>0.0f && Dot(a,c)>0.0f) return 1;
    return 0;
}
/* Swept sphere (start -> end = start + delta, radius) vs triangle tri[3]; fills *out, returns 1 on contact. */
int func_00213658(Vec *start, Vec *end, Vec *delta, float *boxMin, float *boxMax, float radius, float limit, Vec *tri, Hit *out) {
    Vec plane,center;
    Vec *pCenter;

    Hit *best, *edge;
    float distance, d0, d1, denom, t, margin, time;
    int i;
    Plane(&plane,tri);
    distance=Dot(start,&plane);
    distance-=plane.w;
    if(distance<0.0f) return 0;
    d0=distance-radius;
    if(d0<=0.0f) {
        /* already touching the plane at the start */
        out->depth=0.01f-d0;
        AddScaled(&out->pos,start,&plane,out->depth-radius);
        pCenter=&center;
        if(Inside(&out->pos,tri)) {
            COPY_VECTOR(out->normal,&plane);
            out->pos.w=D_0029DA70[0];
            return 1;
        }
    } else {
        float rawTime;
        d1=Dot(end,&plane)-plane.w-radius;
        if(d1>0.0f) return 0;
        denom=d0-d1;
        t=d0/denom;
        margin=0.01f/denom;
        if(t<0.0f || t>1.0f) return 0;
        rawTime=t-margin;
        if(rawTime>limit) return 0;
        time=rawTime;
        AddScaled(&center,start,&plane,-radius);
        AddScaled(&out->pos,&center,delta,t);
        pCenter=&center;
        if(Inside(&out->pos,tri)) {
            COPY_VECTOR(out->normal,&plane);
            out->depth=0.01f-d1;
            time=Max(0.0f,time);
            AddScaled(&out->pos,pCenter,delta,time);
            out->pos.w=time;
            return 1;
        }
    }
    /* face missed: test the three edges, keep the earliest hit */
    { struct { Vec pad[2]; Hit edges[3]; } work;
    Hit *edges;
    if(radius==0.0f) return 0;
    best=0;
    for(i=0, edges=work.edges;i<3;i++) {
        edge=&edges[i];
        if(func_00213250(start,delta,boxMin,boxMax,radius,limit,tri+i,tri+(i+1)%3,edge)) {
            if(!best || edge->pos.w<best->pos.w) {
                best=edge;
                limit=Max(best->pos.w,0.0f);
            }
        }
    }
    if(!best) return 0;
    if(best->pos.w>=0.0f) {
        float inv;
        Vec *normal=&out->normal;
        /* normal = (sphere centre at hit time - contact point) / radius */
        AddScaled(pCenter,start,delta,best->pos.w);
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(out->normal) : "r"(pCenter), "r"(&best->pos));
        inv=1.0f/radius;
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(out->normal) : "r"(normal), "f"(inv) : "$2");
        COPY_VECTOR(out->pos,&best->pos);
    } else *out=*best;
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00213658);
#endif
