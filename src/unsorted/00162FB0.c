/* UNIT 0x00162FB0 0x00163148 */
#include "tween.h"
#include "animation.h"
typedef AnimationHandle Entry;
typedef struct {char pad[0x14]; Entry entries[0x214];} Group;
typedef struct {char pad0[0x48B0]; int x48B0;} Rider;
typedef struct {Rider *x0; int x4; int x8; float xC; float x10; float x14;} Obj;
extern void func_001ACFA0(void *,int);
extern void func_00136030(void *,int);
extern int func_00163328(Obj *);






static __inline__ Entry *get_entry(Group *group, int index) { return (Entry *)((char *)group+(index*16+0x14)); }
void func_00162FB0(Obj *obj) {
 Rider *rider=obj->x0;
 Group *group=(Group *)((char *)rider+0x1E40);
 Entry temp;
 float speed;
 void *component;
 func_001ACFA0((char *)rider+0x4580,0);
 rider->x48B0=1;
 func_00136030((char *)rider+0x40C0,1);
 if(func_00163328(obj)) {
  component=(char *)rider+0x3530;
  temp=*get_entry(group,obj->x8); func_00203E10(&temp,obj->x14);
  func_00202E38(&group->entries[obj->x8],1,1,obj->x10);
  speed=obj->x10*0.0625f;
  temp=*get_entry(group,obj->x8);
  obj->xC=speed/func_002042F0(&temp,1.0f);
  func_001B1698((RiderBlendController *)((char *)rider+0x3AF0), (int)obj->x10, 0.0f);
  func_001B1698((RiderBlendController *)((char *)rider+0x3C00), (int)obj->x10, 0.0f);
  func_001B3170((RiderSecondaryBlend *)(component), (int)obj->x10, 0.0f);
  func_001AE630((RiderBodyBlend *)((char *)rider+0x47F0), (int)obj->x10, 0.0f);
 }
}
