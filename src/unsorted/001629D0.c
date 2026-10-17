/* UNIT 0x001629D0 0x00162B50 .rodata=0x28A5B0:0x28A5D0 */
typedef struct Obj {float x0,x4,x8,xC;int x10;unsigned char x14;char pad15[3];float x18;int x1C;} Obj;
typedef struct Packed {char bytes[32];} Packed;
typedef struct Input {float x0,x4,x8;int xC;unsigned char x10;} Input;
extern float func_00162D18(int);
int func_001629D0(Obj *dst,Obj *src,int type,int arg3) {
 int size=0;
 switch(type) {
 case 5: *(Packed *)dst=*(Packed *)src; size=32;break;
 case 1: dst->x0=src->x0;dst->x4=src->x4;dst->x8=src->x8;dst->x10=0;dst->x14=0;dst->xC=func_00162D18(arg3);size=12;break;
 case 2: dst->x0=src->x0;dst->x4=src->x4;dst->x8=src->x8;dst->x10=((Input *)src)->xC;dst->x14=((Input *)src)->x10;dst->xC=func_00162D18(arg3);size=20;break;
 case 3: dst->x0=src->x0;dst->x4=src->x4;dst->x8=src->x8;dst->x10=src->x10;dst->x14=src->x14;dst->xC=src->xC;size=24;break;
 case 4: dst->x0=src->x0;dst->x4=src->x4;dst->x8=src->x8;dst->x10=src->x10;dst->x14=src->x14;dst->xC=src->xC;dst->x18=src->x18;size=28;break;
 }
 return size;
}
