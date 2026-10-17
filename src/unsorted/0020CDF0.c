/* UNIT 0x0020CDF0 0x0020D028 .rodata=0x297E20:0x297E40 */
#include "particle.h"
typedef struct { char pad0[0x290]; float level,rise,fall,max; char pad2A0[0x10]; int delay; short hold; short pad2B6; int state; char *target; } Obj;
extern Obj D_004D9720[];
extern char D_00513B00[];
extern int func_0023D608(void),func_00238F60(void);
extern void func_0020C9F0(Obj *,int);
extern void func_002103D8(void *,int,void *);
void func_0020CDF0(void) {
 int i=0,count,dt;
 count=func_0023D608(); dt=func_00238F60();
 while(i<count) {
 { Obj *current=&D_004D9720[i];
 switch(current->state) {
 case 0: case 3: default: break;
 case 1: { Obj *p=&D_004D9720[i]; p->delay-=dt; if(p->delay<0) p->state=2; break; }
 case 2: { Obj *p=&D_004D9720[i]; p->level+=p->rise*(float)dt;
 if(p->level>=p->max) { p->level=p->max; p->state=3; } break; }
 case 4: { Obj *p=&D_004D9720[i]; p->hold-=dt; if(p->hold<0) p->state=5; break; }
 case 5: { Obj *p=&D_004D9720[i]; p->level-=p->fall*(float)dt;
 if(p->level<=0) { p->level=0; p->state=0; if(p->target) func_0022CFB8((Vector4 *)(p->target+0x10), p->target+0x40, 0x25, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0)); } break; }
 }
 }
 { int offset=i*0x2C0;
 Obj *p=(Obj *)((char *)D_004D9720+offset);
 if(p->state>=2) {
 void *manager;
 func_0020C9F0(p,i);
 manager=D_00513B00;
 func_002103D8(manager,i,p);
 if(p->target) func_002103D8(manager,i,(char *)D_004D9720+0xA0+offset);
 }
 }
 i++;
 }
}
