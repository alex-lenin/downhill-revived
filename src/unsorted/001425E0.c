/* UNIT 0x001425E0 0x00142940 .rodata=0x289C20:0x289C40 */
typedef struct Key { int x0,x4,x8,xc; } Key;
typedef struct Obj { char pad0[0x5c]; unsigned int x5c,x60; int x64; Key x68,x78; char pad88[0x164-0x88]; int x164,x168,x16c; } Obj;
extern void func_00203B68(Key *,int,float);
extern void func_002038E0(Key *,int);
extern void func_00203E10(Key *,float);
extern void func_00203EF0(Key *);
extern float func_00203C28(Key *);
static inline void func_001425E0_blend(Obj *o, Key *key, float target) {
 float value;
 *key=o->x78;value=func_00203C28(key);
 value+=(target-value)*0.125f;
 *key=o->x78;func_00203E10(key,0.0f);
 *key=o->x78;func_00203B68(key,1,value);
 if(__builtin_fabsf(value-target)<0.001f) o->x60=o->x5c;
}
void func_001425E0(Obj *o) {
 Key key;
 if(o->x64==0) {
  if(o->x164!=0) {
   if(o->x16c==0) o->x5c=o->x164>=1?5:2;
   else {
    o->x5c+=o->x164;
    if(o->x5c-1>=6U) o->x5c-=o->x164;
    else o->x64=60;
   }
  }
 } else o->x64--;
 if(o->x5c!=o->x60) {
  switch(o->x5c) {
  case 0:
   key=o->x68; func_00203B68(&key,1,0.0f);
   o->x60=o->x5c;break;
  case 1:case 6:
   key=o->x78;func_002038E0(&key,0);
   key=o->x78;func_00203E10(&key,2.0f);
   o->x60=o->x5c;break;
  case 2:case 5:
   key=o->x78;func_002038E0(&key,0);
   key=o->x78;func_00203EF0(&key);
   o->x60=o->x5c;break;
  case 3:
   func_001425E0_blend(o,&key,0.609375f);
  default:
   func_001425E0_blend(o,&key,0.546875f);
   break;
  }
 }
}
