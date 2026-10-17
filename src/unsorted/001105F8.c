/* UNIT 0x001105F8 0x001107D8 */
typedef struct Obj {
 char *x0; char pad4[0x2C0]; float x2C4; char pad2C8[0xD8]; int x3A0,x3A4;
 char pad3A8[0x84]; int x42C; char pad430[4]; int x434; char pad438[8]; unsigned long x440;
} Obj;
extern int func_00238F60(void);
extern float func_00238F38(void);
extern void func_00111150(Obj *,int);
extern int func_00110EF8(Obj *,int);
extern void func_00110F88(Obj *,int);
extern int func_001E64C8(void *);
extern int func_00138560(void *);
extern void func_00111080(Obj *,int);
extern int func_00111110(Obj *);
extern void func_00117278(void *);
extern void func_001107D8(Obj *,int);
void func_001105F8(Obj *obj,int arg) {
 char *p;
 if(obj->x42C>0) {
  obj->x42C-=func_00238F60(); func_00111150(obj,arg);
  if(*(unsigned char *)(obj->x0+0x61E6)) {
   obj->x42C=0;
   if(!(obj->x440 & 0xC000000000000000UL)) {
    if(func_00110EF8(obj,obj->x3A4)) func_00110F88(obj,obj->x3A4);
   }
  }
 } else if(obj->x440 & 0x100000000000UL) {
  obj->x2C4+=1.0f/func_00238F38();
  if(func_001E64C8(obj->x0+0x6610)) {
   if(!(obj->x440 & 0x4000000000000000UL)) {
    if((obj->x3A0>>10)&1) func_00111080(obj,arg);
   } else if(func_00111110(obj)) {
    func_00111150(obj,arg); obj->x434+=func_00238F60();
   }
  } else if(!(func_00138560(obj->x0+0x40C0)&255)) obj->x440 &= ~0x100000000000UL;
  obj->x440 &= ~0x800000000UL;
 } else if(!func_001E64C8(obj->x0+0x6610)) {
  p=(char *)obj+0x2D4; func_00117278(p);
  if(!*(int *)(p+0xAC)) func_001107D8(obj,arg);
 }
}
