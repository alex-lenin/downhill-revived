/* UNIT 0x00218720 0x002189F8 */
#include "vif_transforms.h"
typedef struct { int x0,x4,x8,xC,x10,x14; } Packet;
extern unsigned int D_0029E1A0,D_0029E1A4,D_0029E1A8,D_0029E1AC,D_0029E1B0,D_0029E1B4;
extern int D_0029E1B8,D_0029E1C0,D_0029E1C8,D_0029E1CC;
extern Packet *D_0029E1DC,*D_0029E1E0;
extern void *D_0029E1E8;
extern char D_00528360[],D_00528980[],D_00580180[],D_005D7980[],D_0061D980[],D_0061F9C0[];
extern Packet D_00528900;
extern void func_00216DD0(void),func_0022F570(void *,int,int),func_0022F588(void *,int,void *),func_0022F600(int),func_00230058(int),func_00216D28(void *,int),func_0021CB98(int,int,void *,int,float,float),func_0021B8A8(int,void *,int),func_0025A960(int),func_002433A8(int,int),func_0021E9A8(void);
extern float func_0020EB30(int),func_0020EE10(int);
extern int func_0023CF78(int),func_0021E3C8(void),func_0022FD90(void),func_00242FA0(void);
#define ADDR(x) (((int)(x)&0xfffffff)|0x30000000)
void func_00218720(int arg0,int arg1,int arg2,int arg3) {
 char buffer[16]; int a,b,c,t; void *e,*f,*g; Packet *packet;
 D_0029E1DC=(Packet *)0x70000300;
 D_0029E1E0=(Packet *)0x70000290;
 *(int *)0x700002b4=0; *(int *)0x700002b8=0;
 func_00216DD0();
 func_0022F570(D_0029E1DC,ADDR(D_00528980),ADDR(D_00580180));
 func_0022F570(D_0029E1E0,ADDR(D_005D7980),ADDR(D_005D7980));
 func_0022F588(D_0029E1E0,ADDR(D_0061D980),D_0061F9C0);
 func_0022F600(0x70000000);
 D_0029E1A0=0x70000400; D_0029E1A4=0x70000900;
 D_0029E1A8=0x70000e00; D_0029E1AC=0x70001300; D_0029E1B0=0x70001d00;
 D_0029E1B4=0x70000350;
 D_0029E1B8=0; D_0029E1C0=0;
 *(int *)0x7000006c=0; *(int *)0x70000068=0;
 *(float *)0x700001d0=func_0020EB30(arg3);
 func_00230058(arg3); func_00216D28(D_0029E1DC,arg3);
 D_0029E1E8=D_00528360+arg3*240;
 a=func_0023CF78(arg3); b=func_0021E3C8(); c=func_0022FD90();
 func_0021CB98(a,b,D_0029E1E8,c,func_0020EE10(arg3),1.0f);
 *(int *)0x700000e0=func_00242FA0();
 __asm__ volatile("sync\n\tsync.p" ::: "memory");
 *(int *)0x70000038=arg0; PrepareVifTransformsAddress(arg0,D_0029E1E8,buffer); func_0021B8A8(arg0,buffer,0);
 t=*(int *)0x7000006c; D_0029E1C8=t; D_0029E1CC=*(int *)0x70000068;
 if(t) {
 packet=D_0029E1DC;
 D_00528900.x0=packet->x0;
D_00528900.x4=packet->x4;
D_00528900.x8=packet->x8;
D_00528900.xC=packet->xC;
D_00528900.x10=packet->x10;
D_00528900.x14=packet->x14;
 func_0025A960(0); func_002433A8(0,0); func_0021E9A8();
 }
}
