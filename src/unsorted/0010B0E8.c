/* UNIT 0x0010B0E8 0x0010B290 */
#include "rider_geometry.h"
typedef struct {char pad[0x5bb4];int x5bb4;char pad2[0x24];int x5bdc;} Settings;
typedef struct {char *x0;char pad[0x39c];int x3a0;} Obj;
extern Settings D_00398C40;
extern void func_00113810(Obj *);

extern void func_00111B18(Obj *);
void func_0010B0E8(Obj *o) {
 int n,n2;float scale;char *p;
 func_00113810(o);
 if(D_00398C40.x5bb4==0)n=D_00398C40.x5bdc/2;
 else if(D_00398C40.x5bb4==2)n=D_00398C40.x5bdc/2+50;
 else n=D_00398C40.x5bdc/2+25;
 func_00156568((RiderSpeedScale *)(o->x0+0x3fd0), (float)n*0.012499999f+0.75f);
 p=o->x0+0x2e94;
 if(D_00398C40.x5bb4==0)n2=D_00398C40.x5bdc/2;
 else if(D_00398C40.x5bb4==2)n2=D_00398C40.x5bdc/2+50;
 else n2=D_00398C40.x5bdc/2+25;
 scale=(float)n2*0.01f+0.5f;
 switch(D_00398C40.x5bb4) {case 0:scale*=0.75f;break;case 2:scale*=1.5f;break;}
 if((o->x3a0>>11)&1)scale*=1.4f;
 *(float*)(p+0x24)=scale;
 func_00111B18(o);
}
