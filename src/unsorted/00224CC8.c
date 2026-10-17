/* UNIT 0x00224CC8 0x00224F58 */
#include "gs_packet.h"
typedef struct {unsigned char r,g,b,a;} Color;
typedef struct {float r,g,b,a;} FloatColor;
typedef struct {char pad0[0x27c0]; int flags; char pad27c4[0x16]; short x27da; char pad27dc[0x3c]; int count,index; char pad2820[4]; int x2824; Color color; char pad282c[4]; void *x2830,*x2834,*x2838; char pad283c[4]; int x2840; char pad2844[0x10]; int x2854;} Obj;
extern Color D_002694D0[];


extern int func_002249A8(int,int);
void func_00224CC8(Obj *obj) {
 char *p=(char *)(((unsigned int)obj+12)&~15U), *base=p;
 int count; FloatColor *out; Color *src;
 obj->x2840=0; obj->x2854=256;
 p=(char *)func_002246A0((unsigned int *)(base),obj->x2824,0);
 count=obj->count;
 *(int *)p=(count<<16)|0x6c004208; p+=4;
 out=(FloatColor *)p; src=&D_002694D0[obj->index];
 if(obj->flags&256) {
 float alpha=(float)obj->color.a;
 float red=(float)obj->color.r, green=(float)obj->color.g, blue=(float)obj->color.b;
 int n;
 alpha*=0.0078125f;
 if(count>0) {n=count; do {out->r=red; out->g=green; out->b=blue; out->a=(float)src->a*alpha; src++; out++;}while(--n!=0); }
 }else {
 int n;
 if(count>0) {n=count; do {out->r=(float)src->r; out->g=(float)src->g; out->b=(float)src->b; out->a=(float)src->a; src++; out++;}while(--n!=0); }
 }
 p+=count*16;
 *(int *)(p+0)=0; *(int *)(p+8)=0x6c020218; *(int *)(p+4)=0;
 p=(char *)func_00224660((GsPacket *)(p+12),0,0x5151,4,0,0x49,1,0,0);
 /* The unsigned index wraps to the word immediately before p on EE. */
 ((float *)p)[0x3fffffff]=(float)((unsigned int)obj->count-1)/(float)obj->x27da;
 p+=16;
 *(int *)(p+4)=0; *(int *)(p+8)=0; *(int *)p=0;
 p+=16; obj->x2830=p;
 p+=0x1000; *(char **)(base+4)=p;
 *(int *)(p+4)=0; *(int *)(p+8)=0;
 p+=16; obj->x2834=p;
 p+=func_002249A8(obj->x2824,0)*4+0x1000; obj->x2838=p;
}
