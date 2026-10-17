/* UNIT 0x00113588 0x00113650 */
#include "rider.h"
typedef struct Related { char pad[0x3a0]; int x3A0; } Related;
typedef RiderPhysics Obj;
extern int D_004B26AC[];
int func_00113588(Obj *obj,Rider *rider,float distance) {
 float limit;
 int result;
 SceneNodeFlags mask;
 Rider *owner;
 Related *related;
 related=(Related *)rider->x78D4;
 result=0;
 if((!related || !((related->x3A0>>11)&1)) && (D_004B26AC[0]==15 || rider->x7A5C==1 || rider->x798C>=3)) {
 owner=obj->owner;
 limit=10.0f;
 if(owner->x6390==4) limit=40.0f;
 if(!owner->x7A68) {
 mask=0x4000200000000UL;
 if((obj->flags&mask)==mask && !obj->x7C && distance<limit) result=1;
 }
 }
 return result;
}
