/* UNIT 0x00227C60 0x00227D80 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x2600]; float x2600,x2604,x2608,x260C;
    char pad2610[0x27C0-0x2610]; unsigned int x27C0;
    char pad27C4[0x2824-0x27C4]; int x2824;
    char pad2828[0x2830-0x2828]; unsigned int *x2830;
    char pad2834[0x2850-0x2834]; int x2850;
} Obj;
typedef struct Packet { int x0,x4,x8,xC; int x10,x14,x18,x1C,x20; float x24,x28,x2C,x30; int x34,x38,x3C,x40,x44,x48; } Packet;
extern char D_850[];

void func_00227C60(Obj *obj) {
    unsigned int *buf,*p;
    Packet *packet;
    buf=(unsigned int *)(((unsigned int)obj+0xC)&~0xF);
    *buf=((((obj->x2830-buf)-4)>>2)+obj->x2850)|0x20000000;
    packet=(Packet *)(obj->x2830-19);
    packet->xC=obj->x2850|0x8000;
    packet->x24=obj->x2600;
    packet->x28=obj->x2604;
    packet->x2C=obj->x2608;
    packet->x30=obj->x260C;
    packet->x48=(obj->x2850<<16)|0x6C00000C;
    buf=(unsigned int *)buf[1];
    buf[3]=(obj->x2850<<16)|0x6E00000B;
    p=(unsigned int *)((char *)buf+(obj->x2850*4+16));
    while((unsigned int)p&0xF) *p++=0;
    func_002249E0((GsPacket *)(p),obj->x2824,(unsigned int)(D_850),(GsPacket *)(buf),0);
    obj->x27C0|=0x800;
}
