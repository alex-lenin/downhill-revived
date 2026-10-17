/* UNIT 0x0015A968 0x0015AB08 */
#include "npc_motion.h"
typedef NpcMotionActor Obj;
typedef NpcMotionWorld World;
extern World D_002AF2C0;
extern float D_0029C6E4[];
extern int func_0011C0D0(World*,float*,int,float);
extern char *func_0011B180(World*,float*,float*,int,float);
extern float func_001385A8(void*);
extern void func_00159CE0(Obj*,int);
extern void func_0015A890(Obj*);
extern int func_00222BD8(int,int);
float func_0015A968(Obj *obj) {
 float distance=D_0029C6E4[0];
 char *hit;
 if(func_0011C0D0(&D_002AF2C0,(float *)&obj->node->position,0,400.0f)) {
  hit=func_0011B180(&D_002AF2C0,(float *)&obj->node->position,&distance,0,200.0f);
  if(hit && distance<40000.0f) {
   distance=distance-distance*func_001385A8(hit+0x40C0);
   if(distance<2500.0f) func_00159CE0(obj,6);
  }
  func_0015A890(obj);
 } else if(D_002AF2C0.x5019C+200.0f < obj->node->position.z) {
  func_00159CE0(obj,10);
 } else if(obj->state==6) {
  if(func_00222BD8(0,100)==42) func_00159CE0(obj,5);
 } else if(obj->state==2) {
  if(func_00222BD8(0,100)==42) func_00159CE0(obj,5);
 }
 return distance;
}
