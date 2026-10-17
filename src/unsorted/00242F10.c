/* UNIT 0x00242F10 0x00242FA0 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Packet { union { u128 all; struct { int x0,x4,x8,xC; } words; } header; unsigned long x10,x18,x20,x28,x30,x38,x40,x48,x50,x58,x60,x68; } Packet;
extern Packet D_0075ED00[];
void func_00242F10(void) {
 D_0075ED00[0].header.all=0;
 D_0075ED00[0].header.words.x8=0x11000000;
 D_0075ED00[0].header.words.xC=0x50000006;
 D_0075ED00[0].x10=0x1000000000008005UL;
 D_0075ED00[0].x18=0xe;
 D_0075ED00[0].x20=0x44;
 D_0075ED00[0].x28=0x42;
 D_0075ED00[0].x30=0x5040c;
 D_0075ED00[0].x38=0x47;
 D_0075ED00[0].x48=8;
 D_0075ED00[0].x58=0x49;
 D_0075ED00[0].x60=0x1000000;
 D_0075ED00[0].x40=0;
 D_0075ED00[0].x50=0;
 D_0075ED00[0].x68=0x4e;
}
