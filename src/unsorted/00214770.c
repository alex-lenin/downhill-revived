/* UNIT 0x00214770 0x00214A10 .sdata=0x29DA80:0x29DA84 */
/* NOTES: Permitted inline asm implements EE dot products, max.s, SDK vector
 * operations and vector copies. The FLT_MAX start value is a literal: gcc pools
 * it in .sdata (D_0029DA80), and a constant-initialised pseudo gets its live
 * length doubled, which is what gives `nearest` its retail register ($f9). */
typedef struct { float x,y,z,w; } Vec;
typedef struct { Vec pos, plane; float distance; } Hit;
#define DOT(out,a,b) __asm__ volatile("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(out) : "f"((a)->x),"f"((a)->y),"f"((a)->z),"f"((b)->x),"f"((b)->y),"f"((b)->z))
/* SDK vector scale sequence, including the scalar transfer. */
#define SCALE(d,s,k) __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)"::"r"(d),"r"(s),"f"(k):"$2","memory")
#define ADD(d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"::"r"(d),"r"(a),"r"(b):"memory")
#define SUB(d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)"::"r"(d),"r"(a),"r"(b):"memory")
#define COPY(d,s) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(d):"r"(s):"$2")
int func_00214770(Vec *start, Vec *end, Vec *planes, int count, Hit *hit, float *lower, float *upper, float radius) {
    float nearest=3.402823466e+38f;
    Vec *nearPlane=0,*enterPlane=0,*exitPlane=0;
    float hi,lo=0.0f,exitDist,enterDist;
    Vec temp __attribute__((aligned(16)));
    if(lower) lo=*lower;
    hi=1.0f;
    if(upper) hi=*upper;
    enterDist=nearest; exitDist=nearest;
    if(count) do {
        float b,a,c,db,da,t;
        int outsideA,outsideB; unsigned char different;
        DOT(a,start,planes); DOT(b,end,planes);
        c=planes->w+radius; da=c-a; db=c-b;
        outsideA=1; if(!(da<0.0f)) outsideA=0;
        outsideB=1; if(!(db>=0.0f)) outsideB=0;
        different=outsideA ^ outsideB; if(different) {
            if(!(da>=0.0f)) return 0;
            if(da<nearest) { nearest=da+0.01f; nearPlane=planes; }
        } else {
            t=(da-0.01f)/(b-a);
            if(da>=0.0f) {
                if(t<hi) { if(t<lo) return 0; hi=t; exitDist=da+0.01f; exitPlane=planes; }
            } else if(lo<t) { if(hi<t) return 0; lo=t; enterDist=db+0.01f; enterPlane=planes; }
        }
        --count; ++planes;
    } while(count);
    if(lower) *lower=lo;
    if(upper) *upper=hi;
    if(!enterPlane) {
        if(exitPlane) { enterPlane=exitPlane; enterDist=exitDist; lo=hi; }
        if(!enterPlane) goto no_enter;
    }
    {
        __asm__("max.s %0,%1,%2":"=&f"(lo):"f"(lo),"f"(0.0f));
        SCALE(&hit->pos,end,lo);
        SCALE(&temp,start,1.0f-lo);
        ADD(&hit->pos,&hit->pos,&temp);
        SCALE(&temp,enterPlane,-radius);
        ADD(&hit->pos,&hit->pos,&temp);
        COPY(hit->plane,enterPlane);
        hit->distance=enterDist; hit->pos.w=lo;
    goto finished;
    }
no_enter:
    {
        SCALE(&hit->pos,nearPlane,radius-nearest);
        SUB(&hit->pos,end,&hit->pos);
        hit->pos.w=-1.0f;
        COPY(hit->plane,nearPlane);
        hit->distance=nearest;
    }
finished:
    return 1;
}
