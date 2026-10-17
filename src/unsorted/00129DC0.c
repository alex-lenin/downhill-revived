/* UNIT 0x00129DC0 0x00129FC8 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct { float x,y,z,w; } Vec;
typedef struct { void *x0; Vec *x4; short x8,xA; } Link;
#define FIELD(p,o,t) (*(t *)((char *)(p)+(o)))
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
extern char D_003041B0[];
extern Vec *func_00163D40(void *,int);
extern void func_002609B0(void *);
#if defined(__mips__)
extern float func_00211980(Vec *, Vec *,int,float);
#endif
extern float func_00222B68(float,float);
extern float func_002237F8(float);
extern void func_00223168(Vec *,Vec *);
extern void func_002235B0(Vec *,Vec *,Vec *);
extern void func_0012AF48(void *,Vec *,Vec *,int);
extern void func_0012B300(void *,Vec *,Vec *);
extern void func_002182A8(void *,int);
void func_00129DC0(void *obj, Vec *input) {
 Vec rotation, pos, zero, normal, quat;
 Link *link; Vec *p, *transform, *plane, *dst, *out, *out2; float w,d;
 if (FIELD(obj,4,void *) == 0) return;
 link=(Link *)((char *)obj+0x6C);
 link->x0=D_003041B0;
 p=input ? input : func_00163D40(D_003041B0,0);
 link->x4=p; link->xA=-1; link->x8=-1;
 transform=(Vec *)((char *)obj+0x80);
 FIELD(transform,0x40,Link *)=link;
 func_002609B0(transform);
 w=rotation.w; COPY_VECTOR(rotation,(char *)obj+0x90); rotation.w=w;
 COPY_VECTOR(pos,input);
 pos.z-=CallGroundProbe(&pos,&normal,0,15.0f);
 plane=(Vec *)((char *)obj+0x130);
 COPY_VECTOR(*plane,&normal);
 __asm__("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(d) : "f"(normal.x),"f"(pos.x),"f"(normal.y),"f"(pos.y),"f"(normal.z),"f"(pos.z));
 FIELD(obj,0x13C,float)=d;
 COPY_VECTOR(FIELD(obj,0x120,Vec),plane);
 __asm__ volatile("sqc2 $vf0, %0" : "=m"(zero));
 d=func_00222B68(-0.17453292f,0.17453292f)*0.5f;
 dst=&quat; quat.x=0; quat.y=0; dst->z=1;
 quat.w=func_002237F8(d);
 func_00223168(&quat,&quat);
 func_002235B0(&rotation,&quat,&rotation);
 FIELD(obj,0xD4,float)=pos.y; FIELD(obj,0xD0,float)=pos.x;
 out=(Vec *)((char *)obj+0xE0); w=out->w; COPY_VECTOR(*out,&zero); out->w=w;
 out2=(Vec *)((char *)obj+0xF0); w=out2->w; COPY_VECTOR(*out2,&rotation); out2->w=w;
 func_0012AF48(obj,&rotation,&normal,1);
 COPY_VECTOR(FIELD(FIELD(obj,4,void *),0x10,Vec),&pos);
 if (FIELD(obj,8,void *)) {
 COPY_VECTOR(FIELD(FIELD(obj,8,void *),0x10,Vec),&pos);
 func_0012B300(obj,&rotation,&normal);
 }
 func_002182A8(FIELD(obj,4,void *),1);
 func_002182A8(FIELD(obj,8,void *),1);
}
