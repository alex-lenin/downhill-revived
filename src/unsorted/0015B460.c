/* UNIT 0x0015B460 0x0015B538 */
typedef struct Sound { int x0,x4,x8,xC; } Sound;
typedef struct Anim { int x0,x4; short x8,xA; } Anim;
typedef struct Obj { char pad[8]; int x8; char padC[0x3C]; Sound x48; char pad58[0x80]; Anim xD8; char padE4[0xD4]; float x1B8,x1BC; char pad1C0[0x22]; signed char x1E2; char pad1E3; short x1E4; } Obj;
extern int func_00163D70(int,int);
extern void func_00159CE0(Obj *,int);
extern void func_002182A8(int,int);
extern float func_00235EA8(float,float,float,float);
extern void func_00203E10(Sound *,float);
void func_0015B460(Obj *obj) {
 Anim *anim; Sound sound; int frame; float value,goal,result;
 if(obj->x1E2) {
  anim=&obj->xD8;
  frame=func_00163D70(anim->x0,anim->x4);
  if(anim->xA==frame) {
   func_00159CE0(obj,10); func_002182A8(obj->x8,0);
  }
  value=obj->x1B8; goal=obj->x1BC;
  obj->x1E4=10; obj->x1E2=0;
  if(value<goal) {
   result=func_00235EA8(value,goal,0.2f,0.1f);
   obj->x1B8=result; sound=obj->x48; func_00203E10(&sound,result);
  }
 }
}
