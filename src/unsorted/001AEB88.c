/* UNIT 0x001AEB88 0x001AEC60 */
#include "camera.h"
#include "rider.h"

typedef RiderComponent Parent;
typedef RiderCamera Obj;
typedef RiderCameraRig State;
extern unsigned char D_0029D3F0;
extern int func_00130010(Rider*); extern void func_001AEC60(State*,Parent*); extern void func_001AEED0(State*,Obj*); extern void func_002185C0(void*,int);
void func_001AEB88(State *state,Obj *obj){int active,flag=0;Parent *parent=obj->xA10;void *rider=parent->rider->bodyNode;func_001AEED0(state,obj);func_00221930((Matrix4 *)((char *)state->xA0+0x40),(Matrix4 *)((char*)state+0x60),(Matrix4 *)((char*)rider+0x40));func_001B3E98(obj,(char *)state->xA0+0x10);active=parent->rider->x7984!=0;if(func_00130010(parent->rider)||D_0029D3F0)flag=1;if(active||flag){func_002185C0(state->xA0,0);func_002185C0(rider,0);}func_001AEC60(state,parent);}
