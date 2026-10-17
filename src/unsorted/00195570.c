/* UNIT 0x00195570 0x00195640 */
#include "rtc.h"
typedef struct Small { unsigned int x0; unsigned short x4; unsigned char x6; } Small;
typedef struct Obj { char pad[0xA4]; int xA4; short xA8,xAA; unsigned char xAC,xAD,xAE,xAF; Small xB0; } Obj;
typedef struct Global { char pad[0xB17C]; int xB17C; Small xB180; } Global;
extern Global D_004B25A0;
extern Small D_0029D098[];

extern void func_00254544(Small *,Small *);
void func_00195570(Obj *obj,int arg1,int arg2,int arg3,int arg4,int arg5) {
 unsigned char tmp[8];
 Global *g;
 ReadRtcLegacy(tmp,arg1);
 obj->xAC=tmp[5];
 obj->xAD=tmp[6];
 obj->xAE=tmp[7];
 obj->xA4=arg2;
 obj->xA8=arg3;
 obj->xAA=arg4;
 obj->xAF=arg5;
 g=&D_004B25A0;
 if(g->xB17C) func_00254544(&obj->xB0,(Small *)((char *)g+0xB180));
 else { obj->xB0.x0=D_0029D098[0].x0; obj->xB0.x4=D_0029D098[0].x4; obj->xB0.x6=D_0029D098[0].x6; }
}
