/* UNIT 0x001BEE28 0x001BF0B8 .rodata=0x28F610:0x28F7D0 */
#include "soft_double.h"
struct Position {char pad[0x18];float x18;};
class Base {public:virtual void Method1();virtual void Method2();virtual void Method3();virtual Position *GetPosition();};
class Target:public Base {public:unsigned int **x4;};
struct Player {char pad[0x792c];Position *x792C;};
struct Owner {int x0,x4,x8,xC;};
extern struct {char pad[0x288];int x288;char pad28C[0x14];int x2A0;} D_004B25A0;
struct World {char pad[0x50130];Player *x50130[1]; Player **Players(){return x50130;} };
extern World D_002AF2C0;
struct Game {char pad[0x5350];char x5350[1]; void *Manager(){return x5350;} };
extern Game D_00398C40;
extern int D_0029D56C,D_0029D580;
extern int func_001BC618(void *,int);extern int func_00222B08(int,int);
extern void func_001BC810(void *,int),func_001BF0B8(Owner *,Target *,int),func_001BF368(Owner *,Target *,int),func_001BF4E8(Owner *,Target *,int),func_001BF218(Owner *,Target *);
void func_001BEE28(Owner *owner,Target *target,int count,int unused,int active) {
 double value;int i,allowed,type,type2,timeout;void *manager,*manager2;
 if(!target || !count)return;
 if(D_004B25A0.x2A0==20){
 for(i=0;i<D_004B25A0.x288;i++){
 value=func_001FD300(target->GetPosition()->x18-D_002AF2C0.x50130[i]->x792C->x18);
 if(func_001FCDA0(value,0)<0)value=func_001FC810(0,value);
 if(func_001FCDA0(value,func_001FCDF0(D_0029D56C))>0)return;
 }
 }
 switch(**target->x4>>18){
 case 4000:case 4005:case 4010:case 4015:case 4020:case 4025:case 4090:case 4095:case 4100:case 4105:case 4110:allowed=1;break;
 default:allowed=0;break;
 }
 if(!allowed)return;
 if(count>=D_0029D580){
 manager=D_00398C40.Manager();type=func_001BC618(manager,owner->x0);
 if(type==5){if(active)func_001BF0B8(owner,target,count);}else if(!active)func_001BC810(manager,type);
 manager2=D_00398C40.Manager();type2=func_001BC618(manager2,owner->x8);
 if(type2==5){if(active)func_001BF368(owner,target,count);}else if(!active)func_001BC810(manager2,type2);
 }
 type=func_001BC618(D_00398C40.Manager(),owner->xC);timeout=300;if(active)timeout=120;
 if(type==5){if(func_00222B08(0,timeout)==30)func_001BF4E8(owner,target,count);}
 if(func_00222B08(0,240)==30)func_001BF218(owner,target);
}
