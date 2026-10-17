/* UNIT 0x00125598 0x001257B8 */
#include "model.h"
struct Vec { float x,y,z,w; };
struct Transform { char pad[16]; Vec x10; };
class Part { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual Transform *GetTransform(); };
struct Target { char pad[22]; short x16; };
struct State { int x0; Target *x4; short x8,xA; };
struct Obj { char pad[8]; int x8; char padC[8]; int x14; char pad18[4]; int x1C,x20; char pad24[16]; float x34,x38; char pad3C[4]; int x40[60]; Part *x130; char pad134[0x38]; State x16C; };
extern "C" {


float func_00236088(float,float,float,float);
void func_001257B8(Obj *,float);
void func_00203E10(Vec *,float);
int func_00163D70(int,Target *);
float func_002223F0(void *,Target *);
void func_00126B20(Obj *,int);
int func_001246D0(Obj *);
}
struct ByteBool { unsigned char v; };
static inline void update(Obj *obj,int divisor) { if(!obj->x1C && !obj->x20) {obj->x38=17.6f/divisor; obj->x34=func_00236088(obj->x34,obj->x38,0.1f,0.001f);} }
static inline void copyCall(Vec *dst, Vec *src) { *dst=*src; func_00203E10(dst,1.0f); }
static inline unsigned char close(State *state, void *pos) {
 if(state->xA==func_00163D70(state->x0,state->x4) && func_002223F0(pos,state->x4)<5.0f) return 1;
 if(state->x4->x16==0 && func_002223F0(pos,state->x4)<5.0f) return 1;
 return 0;
}
static inline unsigned char active(State *state) { if(state->xA==func_00163D70(state->x0,state->x4)) return 1; if(state->x4->x16==0) return 1; return 0; }
extern "C" void func_00125598(Obj *obj) {
 Vec pos; Vec *source;
 void *other;
 if(func_00192BA8((int)(func_002623C0(obj->x130)))==3) update(obj,3);
 else update(obj,3);
 func_001257B8(obj,obj->x34);
 source=(Vec *)((char *)obj->x130+(obj->x40[obj->x8]*16+16));
 copyCall(&pos,source);
 other=obj->x130? &obj->x130->GetTransform()->x10:0;
 if(close(&obj->x16C,other)) {
  obj->x14=1;func_00126B20(obj,1);func_001246D0(obj);
 } else if(active(&obj->x16C)) {
  if(func_001246D0(obj)) {obj->x14=1;func_00126B20(obj,1);}
 }
}
