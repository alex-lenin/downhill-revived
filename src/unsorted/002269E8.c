/* UNIT 0x002269E8 0x00226B90 */
#include "gs_packet.h"
struct Obj { char p0[0x274C]; float x274C; char p2750[12]; int x275C; char p2760[0x68]; float x27C8; char p27CC[8]; float x27D4; char p27D8[0x18]; float x27F0,x27F4; char p27F8[0x20]; unsigned int x2818; unsigned char *x281C; int x2820,x2824; char p2828[8]; void *x2830; int x2834,x2838; char p283C[0x18]; int x2854; };
struct Packet { float x0; int x4; float x8,xC,x10,x14,x18,x1C; int x20,x24; };


extern void func_002249A8(int,int);
void func_002269E8(struct Obj *o) {
 unsigned int *p; float *dst; unsigned char *src; unsigned int i;
 
 o->x2854=0x81;
 p=(unsigned int *)func_002246A0((unsigned int *)(((unsigned int)o+12)&~15U),o->x2824,0);
 *p++=0x6C10008A;
 dst=(float *)p; src=o->x281C; i=0;
 do { i++;dst[0]=(float)src[0];dst[1]=(float)src[1];dst[2]=(float)src[2];dst[3]=(float)src[3];src+=4;dst+=4;}while(i<16);
 p+=64; *p++=0x6C03009A;
 p=(unsigned int *)(struct Packet *)func_00224660((GsPacket *)(p),0,0x42412,5,0,0x5C,1,0,0);
 ((struct Packet *)p)->x0=o->x27C8; ((struct Packet *)p)->x4=o->x275C; ((struct Packet *)p)->x8=o->x27F0; ((struct Packet *)p)->xC=138.0f;
 ((struct Packet *)p)->x10=o->x27D4;((struct Packet *)p)->x14=o->x274C;((struct Packet *)p)->x18=o->x27F4;((struct Packet *)p)->x1C=(float)(o->x2818-1);((struct Packet *)p)->x20=0;
 p+=10; o->x2830=p;
 func_002249A8(o->x2824,0);
 o->x2834=0;o->x2838=0;
}
