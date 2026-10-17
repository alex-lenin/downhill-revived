/* UNIT 0x0017B848 0x0017BA30 */
#include "rider_geometry.h"
typedef struct RiderMarkerHud { char pad[0xC80]; int *xC80; } Obj;
typedef struct State { char pad[0x288]; int x288; } State;
extern State D_004B25A0;
void func_0017B848(Obj *obj,int x,int y,int color,int flags,float alpha) {
    int w,h;
    *(unsigned long*)obj->xC80=0x4021C00000008001UL;
    *(unsigned long*)(obj->xC80+2)=0x5551;
    obj->xC80+=4;
    if(D_004B25A0.x288>=3) {
        if(color==1) { obj->xC80[0]=0x33; obj->xC80[1]=0x4D; obj->xC80[2]=0xCD; }
        else if(color==2) { obj->xC80[0]=0xD4; obj->xC80[1]=0x3E; obj->xC80[2]=0x3E; }
        else if(color==3) { obj->xC80[0]=0xFA; obj->xC80[1]=0xFF; obj->xC80[2]=0; }
        else { obj->xC80[0]=0; obj->xC80[1]=0xB3; obj->xC80[2]=0x33; }
    } else {
        if(flags==0) { obj->xC80[0]=0x50; obj->xC80[1]=0x50; obj->xC80[2]=0x96; }
        else { obj->xC80[0]=0x78; obj->xC80[1]=0x28; obj->xC80[2]=0x28; }
    }
    obj->xC80[3]=(int)alpha;
    obj->xC80+=4;
    if(D_004B25A0.x288>=3) { w=8;h=4; } else { w=12;h=6; }
    obj->xC80[0]=(x-w)<<4;
    obj->xC80[1]=(y-h)<<4;
    obj->xC80[2]=0xFFFF28;
    obj->xC80[3]=0;
    obj->xC80+=4;
    obj->xC80[0]=x<<4;
    obj->xC80[1]=y<<4;
    obj->xC80[2]=0xFFFF28;
    obj->xC80[3]=0;
    obj->xC80+=4;
    obj->xC80[0]=(x+w)<<4;
    obj->xC80[1]=(y-h)<<4;
    obj->xC80[2]=0xFFFF28;
    obj->xC80[3]=0;
    obj->xC80+=4;
}
