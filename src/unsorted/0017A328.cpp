/* UNIT 0x0017A328 0x0017A678 */
#include "format.h"
typedef struct Rider { char pad[0x661C]; float x661C; char tail[0x15B0]; } Rider;
typedef struct World { Rider riders[8]; char pad[0x501A8-8*0x7BD0]; int x501A8; } World;
typedef struct Obj { char pad[0x1164]; Rider *x1164; char pad1168[0x118]; int x1280; char pad1284[0xDB]; unsigned char x135F,x1360; char pad1361[3]; int x1364; } Obj;
typedef struct Mode { char pad[0x38]; int x38,x3C; } Mode;
typedef struct Flags { char pad[0x1219C]; unsigned char x1219C,x1219D,x1219E; } Flags;
extern Mode D_004C3740;
extern World D_002AF2C0;
typedef struct Names { char pad[0x1C]; int ids[8]; int *getNames() { return ids; } } Names;
extern Names D_0043EEC0;
extern Flags D_004225A0;
extern char D_0028BC68[],D_0028BC78[],D_0028BC90[],D_0028BCA0[],D_0028BCB0[],D_0028BCC8[],D_0028BCD8[],D_0028BCF0[],D_0028BD08[];
extern "C" void func_0023D650(int,int *,int *);
extern "C" void func_0020FB80(int,int,int,int,int);
extern "C" void func_0020F588(int,int,int,int,int,int);
extern "C" void func_0020F9F0(int,int,int,int,char *);

void func_0017A328(Obj *o,int delta) {
 char buf[0x20]; int width,height;
 int center,y,next,i,count; Rider *r,*start; int *names,*first;
 func_0023D650(o->x1280,&width,&height);
 if(o->x1364>0) {
  center=width/2; y=72;
  func_0020FB80(0,384,288,-123,-32);
  func_0020F588(o->x1280,0,99,104,206,o->x1364);
  if(D_004C3740.x38==1) {
   if(D_004C3740.x3C) {
    func_0020F9F0(0,o->x1280,center,72,D_0028BC68);
    y=87;
    func_0020F9F0(0,o->x1280,center,87,D_0028BC78);
   } else func_0020F9F0(0,o->x1280,center,72,D_0028BC90);
  } else if(D_004C3740.x3C) {
   func_0020F9F0(0,o->x1280,center,72,D_0028BCA0);
   y=87;
   func_0020F9F0(0,o->x1280,center,87,D_0028BCB0);
  } else func_0020F9F0(0,o->x1280,center,72,D_0028BCC8);
  start=D_002AF2C0.riders; count=D_002AF2C0.x501A8; first=D_0043EEC0.getNames();
  if(count>0) { names=first; r=start; i=count; do {
   if(r==o->x1164) {
    FormatHmsDiscard(*names, (char *)(buf));
    func_0020F9F0(0,o->x1280,center,y+24,buf);
   }
   names++;i--;r++;
  } while(i); }
  y+=36;
  if(o->x135F==0 && (int)o->x1164->x661C>40000) {
   func_0020F588(o->x1280,0,221,219,10,o->x1364);
   func_0020F9F0(0,o->x1280,center,y,D_0028BCD8);
   func_0020F9F0(0,o->x1280,center,y+10,D_0028BCF0);
   y+=20; D_004225A0.x1219C=1;
  }
  if(o->x1360==0 && (int)o->x1164->x661C>60000) {
   func_0020F588(o->x1280,0,221,219,10,o->x1364);
   func_0020F9F0(0,o->x1280,center,y,D_0028BCD8);
   func_0020F9F0(0,o->x1280,center,y+10,D_0028BD08);
   D_004225A0.x1219E=1;
  }
  o->x1364+=delta;
  if(o->x1364>=170) o->x1364=170;
 }
}
