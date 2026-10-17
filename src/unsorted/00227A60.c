/* UNIT 0x00227A60 0x00227C60 */
#include "gs_packet.h"
typedef struct Obj {char pad0[0x2600]; float x2600,x2604,x2608,x260C; char pad2610[0x204]; int x2814; char pad2818[12]; int x2824; char pad2828[8]; void *x2830,*x2834,*x2838,*x283C; char pad2840[0x14]; int x2854;} Obj;
typedef struct Entry {char pad0[12]; unsigned short xC; char padE[2];} Entry;
typedef struct Packet {int x0,x4,x8,xC; char pad10[12]; float x1C; int x20; float x24,x28,x2C,x30; char pad34[16]; int x44;} Packet;
extern Entry D_0067DF90[];


extern int func_002249A8(int,int);
void func_00227A60(Obj *obj) {
 char *start,*p; unsigned short height; int mode,mode2;
 p=(char *)(((unsigned int)obj+12)&0xFFFFFFF0);
 start=p;
 obj->x2854=253;
 p=(void *)func_002246A0((unsigned int *)(p),obj->x2824,0);
 *(int *)p=0x6C030008;
 p=(void *)func_00224660((GsPacket *)(p+4),0,3,1,0,0,0,0,1);
 *((float *)p-1)=16.0f;
 height=(obj->x2814+D_0067DF90)->xC;
 ((Packet *)p)->x4=9;
 ((Packet *)p)->x0=((int)(short)height)>>1;
 ((Packet *)p)->x8=((int)(short)height)>>1;
 ((Packet *)p)->xC=(short)height-9;
 mode=0x136; if(obj->x2824!=256) mode=0x176;
 (void *)func_00224660((GsPacket *)(p+16),0,0x4341,4,0,mode,1,1,253);
 ((Packet *)p)->x1C=1023.0f;
 ((Packet *)p)->x20=0x6C02000D;
 ((Packet *)p)->x24=obj->x2600;
 ((Packet *)p)->x28=obj->x2604;
 ((Packet *)p)->x2C=obj->x2608;
 ((Packet *)p)->x30=obj->x260C;
 mode2=0x116; if(obj->x2824!=256) mode2=0x156;
 (void *)func_00224660((GsPacket *)(p+0x34),0,0x5351,4,0,mode2,1,1,253);
 ((Packet *)p)->x44=0x01000104;
 p+=0x4C;
 obj->x2830=p;
 p+=0xFD0;
 *(char **)(start+4)=p;
 ((int *)p)[1]=0; ((int *)p)[2]=0;
 obj->x283C=p+16;
 ((int *)p)[3]=0x01000404;
 ((int *)p)[2]=0; ((int *)p)[1]=0; ((int *)p)[0]=0;
 p+=(func_002249A8(obj->x2824,0)*4)+16;
 obj->x2834=p;
 p+=0xFD0;
 obj->x2838=p;
}
