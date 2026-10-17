/* UNIT 0x001D9750 0x001D9A40 */
#include "team_roster.h"
extern struct {char pad0[0x288];int x288;char pad28C[0x14];int x2A0;} D_004B25A0;
extern void func_001D4E80(void *);

extern void func_001D2CB8(void *);
extern void func_001D1C58(void *);
extern void func_001DB510(void *);
extern void func_001C0B60(void *);
struct Small {char pad[0x20];int values[4];void Set(int i,int v) {values[i]=v;} void Update() {func_001D2CB8(this);}};
struct Large {char pad[0x70];void Update() {func_001D1C58(this);}};
struct Object {char pad[0x10];Small x10,x40,x70,xA0,xD0,x100,x130,x160,x190,x1C0,x1F0,x220,x250;Large x280,x2F0,x360;};
void func_001D9750(Object *object) {
 char *obj=(char *)object;
 int local[4];
 void *p70,*pA0,*pD0,*p100,*p10,*p40,*p280,*p130,*p190,*p2F0,*p1F0,*p250,*p360,*p160,*p1C0,*p220;
 int i;
 if(D_004B25A0.x2A0==15) func_001D4E80(obj);
 
 
 if(D_004B25A0.x2A0==16) {
 func_001DB9E0(0x64A,local);
 for(int i=0;i<D_004B25A0.x288;i++) if(local[i]==1) {object->x70.Set(i,local[i]);object->xA0.Set(i,local[i]);}
 }
 func_001DB9E0(0x648,local);
 for(int i=0;i<D_004B25A0.x288;i++) if(local[i]==1) object->x10.Set(i,local[i]);
 func_001DB9E0(0x64C,local);
 for(int i=0;i<D_004B25A0.x288;i++) if(local[i]==1) object->x70.Set(i,local[i]);
 object->x70.Update();object->xA0.Update();object->xD0.Update();object->x100.Update();object->x10.Update();object->x40.Update();
 object->x130.Update();object->x160.Update();object->x190.Update();object->x1C0.Update();object->x1F0.Update();object->x250.Update();object->x220.Update();
 object->x280.Update();object->x2F0.Update();object->x360.Update();
 if(D_004B25A0.x2A0==11) {func_001DB510(obj+0x3D0);func_001DB510(obj+0x4D0);func_001DB510(obj+0x510);func_001DB510(obj+0x550);func_001DB510(obj+0x590);}
 else if(D_004B25A0.x2A0==15) {func_001DB510(obj+0x410);func_001DB510(obj+0x450);func_001DB510(obj+0x490);}
 if(((volatile __typeof__(D_004B25A0) *)&D_004B25A0)->x2A0!=15) func_001C0B60(obj+0x5D0);
}
