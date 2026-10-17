/* UNIT 0x00214BA0 0x00214EB0 */
#include "spatial.h"
typedef Vector4 Vector;


struct PackedLong { unsigned long value; } __attribute__((packed));
extern float D_0029DA84[],D_0029DA88[];




extern int func_00214550(Vector *,Vector *,float *,float *);


/* Retail schedules this call differently with the canonical float order. */
/* VU0 vector operations require inline asm. */
#define VMINMAX(lo,hi,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvmini.xyz $vf13,$vf11,$vf12\n\tvmax.xyz $vf14,$vf11,$vf12\n\tsqc2 $vf13,-16(%0)\n\tsqc2 $vf14,0(%0)" : : "r"(hi),"r"(a),"r"(b) : "memory")
#define VOP(op,d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\t" op " $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(d),"r"(a),"r"(b) : "memory")
static inline int IsDone(SpatialIterator *p) { return p->x30 >= p->x28; }
int func_00214BA0(SpatialBatchQuery *records,int count)
{
    Vector low,high,radius;
    SpatialIterator iter;
    Vector boxes[count][2];
    int i,j,k,n,total,newTotal;
    unsigned short hit2,hit4;
    SpatialFilter *flags;
    SpatialBatchQuery *rec;
    Vector *box;
    float *filter;
    low.x=low.y=low.z=D_0029DA84[0];
    high.x=high.y=high.z=D_0029DA88[0];
    for(k=0;k<count;k++) {
        radius.x=radius.y=radius.z=records[k].x10;
        VMINMAX(&boxes[k][0],&boxes[k][1],records[k].x0,records[k].x4);
        VOP("vadd.xyz",&boxes[k][1],&boxes[k][1],&radius);
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,-16(%0)" : : "r"(&boxes[k][1]),"r"(&boxes[k][0]),"r"(&radius) : "memory");
        VOP("vmini.xyz",&low,&low,&boxes[k][0]);
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvmax.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,16(%0)" : : "r"(&low),"r"(&high),"r"(&boxes[k][1]) : "memory");
        records[k].x24=0;
    }
    func_00212048(&iter);
    total=0;
    flags=func_00210950();
    if(flags) iter.cell.filter=*flags;
    func_00212098(&iter,&low,&high,0.0f);
    while(!IsDone(&iter)) {
        for(i=0;i<count;i++) {
            rec=&records[i];
            if(rec->x24 == rec->x1C) continue;
            box=&boxes[i][0];
            filter=(float *)iter.cell.entry;
            if(filter && !func_00214550(box,box+1,filter,filter+3)) continue;
            {
                if(rec->x14==0) n=func_00210A18(iter.collider,&iter.reference,rec->x0,rec->x4,rec->x10,rec->x8,rec->xC,rec->x20+rec->x24,rec->x1C-rec->x24);
                else n=QueryColliderSweepLegacy(iter.collider,&iter.reference,rec->x0,rec->x4,rec->x14,rec->x10,rec->x18,rec->x20+rec->x24,rec->x1C-rec->x24,rec->x8,rec->xC);
                newTotal=total+n;
                if(n) {
                    hit2=iter.x68; hit4=iter.x6A;
                    for(j=0;j<n;j++) {
                        rec->x20[rec->x24+j].x2=hit2;
                        rec->x20[rec->x24+j].x4=hit4;
                    }
                }
                total=newTotal;
                rec->x24+=n;
            }
        }
        func_00212408(&iter);
    }
    return total;
}
