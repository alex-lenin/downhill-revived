/* UNIT 0x001B4DD0 0x001B5130 */
#include "timer.h"
struct Obj {unsigned char x0,x1,x2,x3,x4,x5,x6,x7,x8;char p9[0x17];int x20;char p24[0x1c];unsigned int x40;char p44[4];char *x48;char p4c[0xc];unsigned int *x58;unsigned int x5c;unsigned char x60;};
extern unsigned int D_0029E10C;
extern int D_0029E110;
extern char D_004B1F70[],D_004B2914[],D_00513B00[],D_00759780[];
extern char *func_0023D5F0(int);
extern void func_00219EC8(void *),func_001B5AB0(struct Obj *,int),func_0021F1B8(void *,int,int,int),func_002433A8(int,int),func_0021F210(int,void *,unsigned int,int,int),func_0025A960(int,unsigned int),func_002184D0(int,int,int),func_0023CB00(int),func_0022E3D0(void),func_00215B68(int,int,int,int),func_002102A8(void *,int,int),func_002103D8(void *,int,void *),func_00210460(void *,int,int),func_002065C0(void),func_001C5DF0(void *);
extern int func_0021F3F8(void);
static inline unsigned int Set(unsigned int *p,unsigned int value) {unsigned int old=*p;*p=value;return old;}
static inline void Increment(unsigned int *p,unsigned int limit) {unsigned int value=*p;if(value<limit) *p=value+2;}
void func_001B4DD0(struct Obj *o) {
 unsigned int saved;char *data;unsigned int *p;
 if(o->x60==0) {
 o->x60=1;data=func_0023D5F0(o->x20);o->x48=data;func_00219EC8((D_0029E10C&1)?data+0x10:data+0x50);
 if(o->x8) {func_001B5AB0(o,6);o->x8=0;}
 if(D_0029E10C>=o->x40) {
 if(o->x3) {if(o->x4) goto done;
 func_0021F1B8(o->x48,D_0029E10C&1,0,o->x6^1);func_002433A8(0,0);
 if(func_0021F3F8()) func_0021F210(0,o->x48,D_0029E10C,D_0029E110,1);else func_0021F210(0,o->x48,D_0029E10C,0,1);
 } else {if(o->x4) goto done;
 func_0025A960(0,D_0029E10C);func_002433A8(0,0);func_0021F1B8(o->x48,D_0029E10C&1,0,o->x6^1);func_002433A8(0,0);
 if(func_0021F3F8()) func_0021F210(o->x20,o->x48,D_0029E10C,D_0029E110,1);else func_0021F210(o->x20,o->x48,D_0029E10C,0,1);
 }}
 if(!o->x4) {
 func_002184D0(0,0,0);func_0023CB00(o->x20);func_0022E3D0();
 if(!o->x5) func_00215B68(o->x20,1,((unsigned char)D_0029E10C^1)&1,D_0029E110);
 func_002102A8(D_00513B00,1,((unsigned char)D_0029E10C^1)&1);
 if(o->x5) {saved=Set(o->x58,0x80);func_002103D8(D_00513B00,o->x20,D_004B1F70);func_00210460(D_00513B00,o->x20,1);func_002433A8(0,0);func_002102A8(D_00513B00,1,((unsigned char)D_0029E10C^1)&1);*o->x58=saved;goto second;}
 if(o->x1) {if(!o->x3) {Increment(o->x58,o->x5c);func_002103D8(D_00513B00,o->x20,D_004B1F70);}
 second: if(o->x1 && !o->x3) {Increment(o->x58,o->x5c);func_002103D8(D_00513B00,o->x20,D_004B1F70);}}
 if(o->x0) func_002065C0();if(o->x2) func_00236C60((TimerPool *)(D_00759780));
 }
 done:func_001C5DF0(D_004B2914);
 }
}
