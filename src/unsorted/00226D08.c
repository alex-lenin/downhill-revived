/* UNIT 0x00226D08 0x00226FA8 */
#include "gs_packet.h"
typedef struct Obj { char pad[0x27C8]; float x27C8; char pad2[0x14]; float x27E0,x27E4; int x27E8; float x27EC; char pad3[0x24]; int x2814,x2818,x281C,x2820,x2824,x2828,x282C; char *x2830,*x2834; int x2838; void *x283C; char pad4[0x10]; int x2850; } Obj;
typedef struct Packet { float x0,x4,x8,xC,x10,x14,x18,x1C; int x20,x24,x28; float x2C,x30,x34,x38,x3C,x40,x44,x48,x4C; } Packet;
typedef struct Entry { char pad[12]; short xC,xE; } Entry;
extern Entry D_0067DF90[];
extern unsigned char D_002694D0[][4];
extern char D_1F50[];



extern int func_002249A8(int,int);
void func_00226D08(Obj *obj) {
    void *base; char *cursor; unsigned char *color;
    float width, height, depth, nearval, scale, product, difference, numerator, factor, bias; int i;
    cursor=(char *)(((unsigned int)obj+12)&~15U); base=cursor;
    obj->x2850=10;
    cursor=(char *)(int *)func_002246A0((unsigned int *)(cursor),obj->x2824,0);
    ((int *)cursor)[0]=0; ((int *)cursor)[1]=0; ((int *)cursor)[2]=0; ((int *)cursor)[3]=0x6C060008;
    cursor=(char *)(Packet *)func_00224660((GsPacket *)(cursor+16),0,0x53531,5,0,0x151,1,0,0);
    obj->x283C=cursor;
    color=(unsigned char *)(obj->x281C*4+(char *)D_002694D0);
    depth=obj->x27EC;
    nearval=obj->x27E4;
    scale=obj->x27C8;
    numerator=1.0f-depth;
    width=(float)((Entry *)(obj->x2814*16+(char *)D_0067DF90))->xC;
    width=width+width;
    difference=nearval-obj->x27E0;
    ((Packet *)cursor)->x0=width;
    height=(float)((Entry *)(obj->x2814*16+(char *)D_0067DF90))->xE*0.5f;
    bias=depth+8388608.0f;
    product=scale*nearval;
    factor=numerator/(scale*difference);
    product=product*factor;
    ((Packet *)cursor)->x8=-4094.0f; ((Packet *)cursor)->xC=2047.0f; ((Packet *)cursor)->x10=1.11f; ((Packet *)cursor)->x14=1.21f; ((Packet *)cursor)->x18=1.543211f;
    ((Packet *)cursor)->x1C=bias;
    ((Packet *)cursor)->x4=height;
    ((Packet *)cursor)->x20=color[0]; ((Packet *)cursor)->x24=color[1]; ((Packet *)cursor)->x28=color[2]; ((Packet *)cursor)->x2C=factor;
    ((Packet *)cursor)->x30=-8388608.0f; ((Packet *)cursor)->x34=-8388608.0f; ((Packet *)cursor)->x38=-8388608.0f; ((Packet *)cursor)->x3C=1.0f-product; ((Packet *)cursor)->x40=0.0625f;
    
    ((Packet *)cursor)->x44=1.0f/obj->x27C8; ((Packet *)cursor)->x48=obj->x27C8*0.0625f; ((Packet *)cursor)->x4C=obj->x27C8;
    cursor+=0x50;
    func_002249E0((GsPacket *)(cursor),obj->x2824,(unsigned int)(D_1F50),(GsPacket *)(base),0);
    cursor+=func_002249A8(obj->x2824,0)*4;
    obj->x2830=cursor+0x40;
    for(i=4;i<8;i++) { __asm__ volatile("sqc2 $vf0, %0" : : "m"(*(char *)(i*16+obj->x2830)) : "memory"); }
    obj->x2834=i*16+obj->x2830;
    for(i=4;i<8;i++) { __asm__ volatile("sqc2 $vf0, %0" : : "m"(*(char *)(i*16+obj->x2834)) : "memory"); }
}
