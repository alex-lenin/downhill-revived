/* UNIT 0x00218B80 0x00218C28 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union { U128 quad; unsigned int word[4]; } Quad;
typedef struct { Quad header; unsigned long x10,x18,x20,x28; } Packet;
typedef struct { unsigned int x0,x4,x8,xC; } Entry;
typedef union { unsigned long all; struct { unsigned int low, high; } words; unsigned short half[4]; } Pair;
extern Entry D_00621310[];
int func_00218B80(Packet *p,int index,int value) {
struct { unsigned int low,high; } pair;
pair.high=D_00621310[index].xC;
p->header.quad=(U128)0x50000002;
*(unsigned short *)&pair.high=(*(unsigned short *)&pair.high&0xc000)|(value&0x3fff);
pair.low=0;
p->header.word[1]=(D_00621310[index].x0+0x10)&0xfffffff;
p->header.word[2]=0x11000000;
p->header.word[3]=0x50000002;
p->x10=((unsigned long)0x8000<<45)|0x8001;
p->x18=0xe;
p->x20=*(unsigned long *)&pair;
p->x28=0x50;
return 3;
}
