/* UNIT 0x0013F098 0x0013F3F0 */
#include "ui_style.h"
typedef struct Obj { int x0; char pad4[8]; int xC,x10; char pad14[0x404]; int x418,x41C,x420; char pad424[0x3C]; int x460,x464,x468; char pad46C[0x4F]; unsigned char x4BB,x4BC; char pad4BD[0x143]; int x600[1]; } Obj;
typedef struct Global { char pad[0x254]; char *x254; } Global;
extern Global *D_0029DE68;
extern char D_0029C4F0[],D_0029C4F8[],D_0029C500[];
extern char **D_0029DD30;
extern int D_003F52F0[];
extern int D_003F52D0[];
extern char *func_0018D860(void *);
extern void func_0020F860();
extern void func_0020F9A8();
extern void func_0020FA98();
extern void func_0020F9F0();
extern int func_00254D68();
static inline int min(int a,int b) { return a<b?a:b; }
static inline int max(int a,int b) { return a<b?b:a; }
void func_0013F098(Obj *o) {
 char buf[0x70], number[0x10];
 int x,width,count,n,a,b,y,step,end,start,i,pos,font;
 width=o->x418; count=o->x41C; n=width-count+1; x=o->x464; a=count-1; a=o->x0-a; b=o->x420; y=o->x460; step=o->x468;
 b=min(o->x0,b);
 start=max(b,a);
 n=min(n,start);
 start=max(n,0);
 o->x420=start;
 if(start>0) {
  func_0018D800((const UiStyle *)(void *)(o->xC));
  if(o->x4BB) func_0020F9A8(0,D_003F52F0[0],x,y-step,D_0029DE68->x254);
  else if(o->x4BC) func_0020FA98(0,D_003F52F0[0],x,y-step,D_0029DE68->x254);
  else func_0020F9F0(0,D_003F52F0[0],x,y-step,D_0029DE68->x254);
 }
 end=width; if(width>=start+count) end=start+count;
 i=start; while(i<end) {
  func_00254D68(number,D_0029C4F0,i+1);
  if(i==o->x0) {
   func_0018D800((const UiStyle *)(void *)(o->x10));
   func_00254D68(buf,D_0029C4F8,number,D_0029DD30[o->x600[i]]);
   func_0020F860(0,buf);
  } else {
   func_0018D800((const UiStyle *)(void *)(o->xC));
   func_00254D68(buf,D_0029C500,number,D_0029DD30[o->x600[i]]);
   func_0020F860(0,buf);
  }
  pos=(i-start)*step+y;
  func_0020F9A8(0,D_003F52D0[8],x,pos,buf);
  font=D_003F52D0[8];
  func_0020FA98(0,font,600,pos,func_0018D860((char *)o+(i*16+0x18))); i++;
 }
 if(end<width) {
  func_0018D800((const UiStyle *)(void *)(o->xC));
  if(o->x4BB) func_0020F9A8(0,D_003F52F0[0],x,(end-start)*step+y,D_0029DE68->x254);
  else if(o->x4BC) func_0020FA98(0,D_003F52F0[0],x,(end-start)*step+y,D_0029DE68->x254);
  else func_0020F9F0(0,D_003F52F0[0],x,(end-start)*step+y,D_0029DE68->x254);
 }
}
