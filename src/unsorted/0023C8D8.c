/* UNIT 0x0023C8D8 0x0023CB00 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0023D4F8_28_func_0023D4F8 func_0023D4F8
#else
#define CallScalarBridge0023D4F8_28_func_0023D4F8 ScalarBridge0023D4F8_28
#endif
typedef struct Desc {float x0;unsigned short x4,x6;short x8,xa;unsigned short xc,xe;char pad10[16];float x20;char pad24[0x9c];} Desc;
typedef struct State {int x0;Desc *x4;int x8,xc,x10;} State;
typedef struct Packet {char pad0[0xc0];unsigned long xc0;char padc8[0x78];unsigned long x140;char pad148[0xe8];unsigned long x230;char pad238[0x78];unsigned long x2b0;char pad2b8[0xa8];} Packet;
typedef struct Entry {char pad0[16];int x10;char pad14[0xdc];} Entry;
extern Desc D_0027ECC0[];
extern State D_0075A9D0[];
extern Entry D_00528360[];
extern Packet D_0075BAC0[];
extern int func_00208FB8(int);
extern int func_00220D70(int);
extern void func_0021F3D8(int);
extern void func_0023D5C0(int *,int *,int *,int *);
extern void func_0021EB30(Packet *,int,int,int,int,int,int);

#if defined(__mips__)
extern void func_0023D4F8(int,int,int,int);
#else

#endif

extern void func_0023D8D8(int,float,float);
#define PACK_RECT(p) ((unsigned long)(p)->xc | ((unsigned long)((p)->xc+(p)->x4-1)<<16) | ((unsigned long)(p)->xe<<32) | ((unsigned long)((p)->xe+(p)->x6-1)<<48))
void func_0023C8D8(int item,int index) {
 State *state=&D_0075A9D0[index];Desc *desc;Packet *packet;int a,b,c,d;
 state->xc=state->x8;
 if(!state->x0) state->x0=func_00208FB8(state->x8);
 state->x8=item;state->x10=state->x0;desc=&D_0027ECC0[item];state->x4=desc;
 ((Entry *)(index*0xf0+(unsigned int)D_00528360))->x10=func_00220D70(0);packet=&D_0075BAC0[index];
 func_0021F3D8(0);
 func_0023D5C0(&a,&b,&c,&d);
 func_0021EB30(packet,1,desc->x8,desc->xa,2,0x31,1);
 CallScalarBridge0023D4F8_28_func_0023D4F8(a,b,c,d);
 packet->xc0=PACK_RECT(desc);
 packet->x230=PACK_RECT(desc);
 packet->x140=PACK_RECT(desc);
 packet->x2b0=PACK_RECT(desc);
 func_0023D8D8(index,desc->x0,desc->x20);
}
