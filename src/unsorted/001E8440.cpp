/* UNIT 0x001E8440 0x001E86E8 */
#include "animation.h"
typedef AnimationHandle Vec;
typedef struct { char pad[0x14]; Vec vec[1]; } Part;
typedef struct { char *x0; int x4,x8,xC; char pad10[0x1c]; int x2C,x30,x34; char pad38[0x194]; int x1CC; char pad1D0[0x21]; unsigned char x1F1; } Obj;
typedef struct { int x0,x4,x8,xC,x10,x14; float x18; char pad1C[0x24]; int x40; char pad44[0x10]; } Entry;
extern Entry D_00272888[];
extern void func_001E9A48(void *,int);
extern int func_001E9208(Obj *);

extern void func_001E86E8(Obj *),func_001E9250(Obj *),func_001EA7C8(Obj *);

extern float func_001E5198(void *);
extern int func_001E7BD8(Obj *);
static __inline__ Vec *get_part(char *part,int index) {return (Vec *)(part+(index*16+0x14));}
static __inline__ Vec *get_base(char *base,int index) {return (Vec *)base+index;}
void func_001E8440(Obj *obj) {
 Vec first, second;
 char *base=obj->x0;
 char *part=base+0x1E40;
 char *other=base+0x6610;
 Entry *entry;
 float scale;
 struct { unsigned char v; } stopped;
 func_001E9A48(base+0x5FF0,obj->x2C);
 obj->xC++;
 entry=&D_00272888[obj->x2C];
 if(entry->x40==-1 && obj->x1F1 && obj->x30==0) obj->x30=func_001E9208(obj);
 first=*get_part(part,obj->x34);
 stopped.v=(func_00204230(&first) ^ 1);
 if(stopped.v) {
  if(entry->x40==-1) {
   if(entry->xC) {obj->x1CC=2;func_001E86E8(obj);return;}
   obj->x1CC=5;func_001E9250(obj);return;
  }
  if(entry->x10) {second=*get_base(base,obj->x34);func_00203CB8(&second);}
  obj->x1CC=2;
  scale=entry->x18*func_001E5198(other)* *(float *)(part+0x211C);
  obj->x34=D_00272888[obj->x2C].x8;
  second=*get_part(part,obj->x34);
  func_00203E10(&second,scale);
  SelectAnimationTimeFirst(&*get_part(part,obj->x34), 1.0f, 1, 1);
  if(entry->x10) {
   second=*get_base(base,obj->x34);
   func_00203E10(&second,scale);
   SelectAnimationTimeFirst((Vec *)(base+obj->x34*16), 1.0f, 1, 1);
  }
 }
 if(func_001E7BD8(obj)) func_001EA7C8(obj);
}
