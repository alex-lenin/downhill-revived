/* UNIT 0x002257F8 0x00225A40 */
#include "gs_packet.h"
typedef struct Obj {
 char pad0[0x275C]; int x275C; char pad2760[0x60]; int x27C0; char pad27C4[4]; float x27C8,x27CC;
 char pad27D0[0x44]; int x2814; char pad2818[4]; int x281C; char pad2820[4]; int x2824;
 unsigned char x2828,x2829,x282A; char pad282B[5]; int *x2830,*x2834,*x2838;
 char pad283C[0x18]; int x2854;
} Obj;
typedef struct Texture { int x0,x4,x8; unsigned short xC,xE; } Texture;
extern unsigned char D_002694D0[][4];
extern Texture D_0067DF90[];


extern int func_002249A8(int,int);
void func_002257F8(Obj *obj) {
 int *p,*q; int address; Texture *texture; unsigned char *color; int count,width,n,remaining;
 int low,high,step;
 address=(int)obj+12;
 p=(int*)(address&0xFFFFFFF0);
 obj->x2854=256;
 q=(int *)func_002246A0((unsigned int *)(p),obj->x2824,0);
 p[3]+=2; p[4]&=0x7fff;
 p=(int *)func_00224660((GsPacket *)(q),0,1,1,0,0x136,1,1,1);
 color=D_002694D0[obj->x281C];
 if(obj->x27C0&256) { p[0]=obj->x2828; p[1]=obj->x2829; p[2]=obj->x282A; }
 else { p[0]=color[0]; p[1]=color[1]; p[2]=color[2]; }
 p[3]=128; p+=4;
 low=0x80000; high=0x880000; step=0x800000;
 texture=&D_0067DF90[obj->x2814];
 count=(int)(texture->xE<<16)>>23;
 width=(((int)(texture->xC<<16)>>23)-1)<<7;
 *p++=(count<<18)|0x6D004108;
 n=count*4-1;
 if(count>0) { remaining=count; do {
  int a=low|8,b=high|0x88,c=high|8,d=low|0x88;
  p[0]=a;p[1]=b;p[2]=c;p[3]=d;
  p[4]=d;p[5]=c;p[6]=b;p[7]=a;
  low+=step; high+=step;
  p+=8;
 } while(--remaining); }
 *p++=0x6C030148;
 (int *)func_00224660((GsPacket *)(p),n,0x4343,4,0,0x136,1,0,0); p+=8;
 p[-1]=width;
 ((float*)p)[0]=obj->x27C8;
 p[1]=0;
 obj->x275C=0;
 ((float*)p)[2]=128.0f;
 ((float*)p)[3]=obj->x27CC*0.5f;
 p[4]=0;
 p+=6;
 obj->x2830=p;
 p=(int*)((char*)p+(func_002249A8(obj->x2824,0)*4+0x1000));
 obj->x2834=p;
 p+=0x400;
 obj->x2838=p;
}
