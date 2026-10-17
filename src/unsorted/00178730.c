/* UNIT 0x00178730 0x001790E8 */
#include "soft_double.h"
typedef struct { char pad0[0x71C]; int x71C; } Sub;
typedef struct { char pad0[0x6610]; Sub sub; } Big;
typedef struct {
 char pad0[0x1164]; Big *x1164;
 char pad1168[0x118]; int x1280;
 char pad1284[0x20]; int x12A4; int x12A8; int x12AC; int x12B0; int x12B4; int x12B8; int x12BC; int x12C0; int x12C4; int x12C8; int x12CC;
 char pad12D0[4]; int x12D4; int x12D8; float x12DC; int x12E0; int x12E4;
} Obj;
typedef struct { char pad0[0x10C]; int x10C; char pad110[0x178]; int x288; } State;
extern State D_004B25A0;
extern char *D_0026ED40[];
extern char D_0028BBF8[],D_0028BC08[],D_0028BC18[],D_0028BC28[];
extern char D_0029C950[],D_0029C998[],D_0029C9E8[],D_0029C9F0[],D_0029C9F8[];
extern char D_0029CA00[],D_0029CA08[],D_0029CA10[],D_0029CA18[],D_0029CA20[],D_0029CA28[],D_0029CA30[],D_0029CA38[];
extern int func_001E6548(Sub *);
extern int func_001E64E0(Sub *);
extern int func_001E65D0(Sub *);
extern float func_001E51C0(Sub *,int);
extern float func_001E51E8(Sub *,int);

extern int func_0023DED0(void);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F588(int,int,int,int,int,int);
extern int func_00254D68(char *,const char *,...);
extern void func_0017B520(Obj *,char *,float,float);
extern void func_0017B5C8(Obj *,char *,float,float);
extern void func_001782B8(Obj *);

void func_00178730(Obj *obj) {
 char text[64];
 int count,show,fading,big,len,length,quarter,fadeStart,time,trick;
 float scale,alpha,size,fade,x,y,step,value;
 Sub *sub;
 show=0;
 count=func_001E6548(&obj->x1164->sub);
 sub=&obj->x1164->sub;
 scale=sub->x71C;
 trick=func_001E64E0(sub);
 obj->x12B4=func_001E65D0(&obj->x1164->sub);
 if(count) {
  if(obj->x12A4>0 && obj->x12BC<120) {
   obj->x12BC++;
   func_0020F588(obj->x1280,0,110,110,200,180);
   if(D_004B25A0.x288>=3) func_0020FB80(0,224,192,-91,-32);
   else if(D_004B25A0.x288==2) func_0020FB80(0,256,192,-107,-32);
   else func_0020FB80(0,320,224,-139,-32);
   if(scale>1.0f) len=func_00254D68(text,D_0029C9E8,D_0026ED40[obj->x12A4]);
   else len=func_00254D68(text,D_0029C9F0,D_0026ED40[obj->x12A4]);
   if(trick && obj->x12A4<44) func_00254D68(text+len,D_0028BBF8);
   func_0017B520(obj,text,1.0f,0.13f);
  }
  show=1;
  func_0020F588(obj->x1280,0,88,152,73,128);
 } else if(obj->x12A4>=44 && obj->x12BC<120 && obj->x12A4!=46 && !(obj->x12A4>=49 && obj->x12A4<55)) {
  obj->x12BC++;
  func_0020F588(obj->x1280,0,110,110,200,180);
  if(D_004B25A0.x288>=3) func_0020FB80(0,224,192,-91,-32);
  else if(D_004B25A0.x288==2) func_0020FB80(0,256,192,-107,-32);
  else func_0020FB80(0,320,224,-139,-32);
  if(obj->x12A4==47) func_00254D68(text,D_0029C9F8,D_0026ED40[obj->x12A4],obj->x12B0);
  else func_00254D68(text,D_0029C9F8,D_0026ED40[obj->x12A4],obj->x12A8+obj->x12AC+obj->x12B0);
  func_0017B520(obj,text,1.0f,0.13f);
 } else obj->x12A4=0;
 fading=0;
 alpha=128.0f;
 big=1;
 if(obj->x12C4>=0) {
  show=1;
  if(obj->x12CC) length=120;
  else { length=90; big=0; }
  if(D_004B25A0.x288>=2) func_0020FB80(0,256,192,-107,-32);
  else func_0020FB80(0,384,288,-139,-32);
  time=obj->x12C4;
  quarter=length>>2;
  fadeStart=length-quarter;
  if(time<quarter) alpha=(float)time/(float)quarter*128.0f;
  else if(fadeStart<time) {
   fading=1;
   alpha=(1.0f-(float)(time-fadeStart)/(float)(length-fadeStart))*128.0f;
  }
  if(obj->x12CC) func_0020F588(obj->x1280,0,88,152,73,(int)alpha);
  else func_0020F588(obj->x1280,0,100,40,40,(int)alpha);
  func_00254D68(text,D_0029C998,obj->x12C8);
  size=0.25f;
  if(D_004B25A0.x288>=3) size=0.275f;
  else if(D_004B25A0.x288>=2) size=0.2f;
  func_0017B520(obj,text,1.0f,size);
  obj->x12C4++;
  if(length<obj->x12C4) obj->x12C4=-1;
 }
 if(show && D_004B25A0.x10C!=14) {
  fade=128.0f;
  if(fading) fade=alpha;
  if(big) func_0020F588(obj->x1280,0,88,152,73,(int)fade);
  else func_0020F588(obj->x1280,0,100,40,40,(int)fade);
  if(D_004B25A0.x288>=2) func_0020FB80(0,224,192,-91,-32);
  else func_0020FB80(0,256,192,-107,-32);
  x=1.5f; y=0.275f; step=0.08f;
  if(D_004B25A0.x288>=2) {
   if(D_004B25A0.x288==2) {
    if(func_0023DED0()==1) { x=1.175f; y=0.35f; }
    else step=0.12f;
   } else { x=1.175f; y=0.38f; step=0.12f; }
  }
  if(count) {
   obj->x12D4=obj->x12B8; obj->x12D8=obj->x12C0; obj->x12E4=obj->x12B4; obj->x12E0=count; obj->x12DC=scale;
  } else {
   obj->x12B8=obj->x12D4; obj->x12C0=obj->x12D8; obj->x12B4=obj->x12E4; count=obj->x12E0; scale=obj->x12DC;
  }
  if(D_004B25A0.x288<3) {
   func_00254D68(text,D_0029CA00,D_0029CA08,obj->x12A8);
   func_0017B5C8(obj,text,x,y); y+=step;
   if(obj->x12AC>0) { func_00254D68(text,D_0029CA00,D_0029CA10,obj->x12AC); func_0017B5C8(obj,text,x,y); y+=step; }
   if(obj->x12B4>0) { func_00254D68(text,D_0028BC08,D_0029CA18,obj->x12B4); func_0017B5C8(obj,text,x,y); y+=step; }
   if(obj->x12B8>0 && obj->x12C0==0) { func_00254D68(text,D_0029CA20,D_0029C950,obj->x12B8); func_0017B5C8(obj,text,x,y); y+=step; }
   else if(obj->x12C0>0) { value=func_001E51C0(sub,obj->x12C0); func_00254D68(text,D_0028BC18,D_0029C950,func_001FD300(value)); func_0017B5C8(obj,text,x,y); y+=step; }
   if(scale>1.0f) { value=func_001E51E8(sub,(int)scale); func_00254D68(text,D_0028BC18,D_0029CA28,func_001FD300(value)); func_0017B5C8(obj,text,x,y); y+=step; }
   if(count!=obj->x12A8) { func_00254D68(text,D_0029CA00,D_0029CA30,count); func_0017B5C8(obj,text,x,y); }
  } else {
   value=obj->x12C0;
   if(scale>0.0f) value+=func_001E51E8(sub,(int)scale);
   if(value>1.0f) { func_00254D68(text,D_0028BC28,D_0029CA38,func_001FD300(value)); func_0017B5C8(obj,text,x,y); y+=step; }
   func_00254D68(text,D_0029CA00,D_0029CA30,count); func_0017B5C8(obj,text,x,y);
  }
 }
 if(obj->x12BC>=120) obj->x12A4=0;
 func_001782B8(obj);
}
