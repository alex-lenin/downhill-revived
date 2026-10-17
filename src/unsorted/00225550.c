/* UNIT 0x00225550 0x002256F0 */
#include "gs_packet.h"
typedef struct Obj { char pad0[0x2824]; int x2824; unsigned char color[4]; char pad282C[4]; void *x2830,*x2834; char pad2838[0x1C]; int x2854; } Obj;


extern int func_002249A8(int, int);
void func_00225550(Obj *obj) {
 int *packet,*head;

 float root;
 packet=(int *)(((unsigned int)obj+12)&0xFFFFFFF0);
 head=packet;
 obj->x2854=128;
 packet=(int *)func_002246A0((unsigned int *)(head),obj->x2824,0);
 packet[0]=0; packet[1]=0; packet[2]=0x6C020188;
 if(obj->x2824==0) packet=(int *)(float *)func_00224660((GsPacket *)(packet+3),0,0x4441,4,0,3,1,0,0);
 else packet=(int *)(float *)func_00224660((GsPacket *)(packet+3),0,0x4441,4,0,0x43,1,0,0);
 __asm__ volatile("sqrt.s %0,%1" : "=&f"(root) : "f"(0.96f));
 ((float *)packet)[-1]=1.0f/(1.0f-root);
 ((float *)packet)[0]=obj->color[0]; ((float *)packet)[1]=obj->color[1]; ((float *)packet)[2]=obj->color[2]; ((float *)packet)[3]=obj->color[3];
 packet+=5;
 obj->x2830=packet;
 packet+=obj->x2854*4;
 head[1]=(int)packet;
 packet[2]=0;
 head=packet;
 packet+=4;
 packet+=obj->x2854*4;
 head[1]=(int)packet;
 packet[1]=0; packet[2]=0;
 packet+=4;
 packet+=obj->x2854*4;
 packet+=func_002249A8(obj->x2824,0);
 obj->x2834=packet;
}
