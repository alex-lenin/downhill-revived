/* UNIT 0x00113398 0x00113588 */
#include "rider.h"
typedef RiderPhysics Obj;
extern int D_004B26AC[];

extern void func_00112808(Obj *,int,int),func_00112878(Obj *,int,int);
static __inline__ void set_right(Rider *r,int value) {if(r->x13DC) r->x126C=value;else {r->x11A4=value;r->x126C=0;}}
static __inline__ void set_left(Rider *r,int value) {if(r->x13DB) r->x1268=value;else {r->x11A0=value;r->x1268=0;}}
void func_00113398(Obj *o,Rider *r,const Vector4 *arg2,float distance) {
 float pos[4] __attribute__((aligned(16)));float lower,upper;
 if(func_00113588(o,r,distance)) {
 func_00221888(&o->owner->bodyNode->matrix,arg2,(Vector4 *)pos);
 if(o->owner->x6390==4) {
 lower=10.0f;upper=40.0f;
 if(r->x7A5C==1) {upper=0.0f;lower=0.0f;if(D_004B26AC[0]==15) {lower=10.0f;upper=30.0f;}}
 }else if(D_004B26AC[0]==15) {lower=-7.0f;upper=7.0f;}
 else if(r->x7A5C==1) {lower=-10.0f;upper=1.0f;}else {lower=-4.0f;upper=4.0f;}
 if(lower<pos[1] && pos[1]<upper) {
 o->x404=0;
 o->flags |= ((SceneNodeFlags)0x8000<<28);
 o->flags |= ((SceneNodeFlags)0x8000<<27);
 if(pos[0]>0.0f) set_right(o->owner,1);else set_left(o->owner,1);
 if(D_004B26AC[0]!=10) {
 if(r->x7A5C==1) func_00112808(o,r->x7978,2);else func_00112878(o,r->x7978,2);
 }
 }
 }
}
