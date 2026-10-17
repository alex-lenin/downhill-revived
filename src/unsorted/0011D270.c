/* UNIT 0x0011D270 0x0011D488 */
typedef unsigned int u128 __attribute__((mode(TI)));
extern void func_002608C8(void *);
#define I(o) (*(int *)(p+(o)))
#define H(o) (*(short *)(p+(o)))
#define F(o) (*(float *)(p+(o)))
#define Q(o) (*(long long *)(p+(o)))
#define Z(o) __asm__ volatile("sqc2 $vf0, %0" : "=m"(*(u128 *)(p+(o))))
typedef struct { int x0,x4; short x8,xA; } Small;
typedef struct { int x0,x4; float x8; int xC; } Vec;
typedef struct { char pad[0x30]; int x30,x34,x38,x3C; } Blk;
/* sq $zero: gcc materializes a TImode 0 with por first */
#define SQZ(o) __asm__ volatile("sq $0, %0" : "=m"(*(u128 *)(p+(o))))
void func_0011D270(char *p) {
Small *a; Vec *c, *d; Blk *b; float one;
I(4)=0; I(8)=0; I(0xC)=1; I(0x10)=0; I(0x14)=0; Z(0x20); a=(Small *)(p+0xFC);
I(0x30)=0;
I(0x34)=0;
I(0x38)=0;
I(0x3C)=0;
I(0x40)=0;
I(0x4C)=0;
I(0x50)=0;
I(0x5C)=0;
I(0x60)=0;
I(0x6C)=0;
I(0x70)=0;
I(0x7C)=0;
I(0x80)=0;
I(0x8C)=0;
I(0x90)=0;
I(0x9C)=0;
I(0xA0)=0;
I(0xAC)=0;
I(0xB0)=0;
I(0xBC)=0;
I(0xC0)=0;
I(0xCC)=0;
I(0xD0)=0;
I(0xDC)=0;
I(0xE0)=0;
I(0xEC)=0;
I(0xF0)=0;
a->x0=0; a->x4=0; a->xA=0; a->x8=0; func_002608C8(p+0x110); b=(Blk *)(p+0x160);
SQZ(0x160); SQZ(0x170); SQZ(0x170);
one=1.0f;
c=(Vec *)(p+0x1C0); d=(Vec *)(p+0x1D0);
b->x3C=0; b->x38=0; b->x34=0; b->x30=0; I(0x1A0)=0; F(0x1A4)=one;
c->x0=0; c->x4=0; c->x8=one; c->xC=0; d->x0=0; d->x4=0; d->x8=one; d->xC=0; Z(0x1E0); Z(0x1F0);
H(0x268)=0; H(0x26A)=0; H(0x26C)=0;
I(0x200)=0;
I(0x204)=0;
I(0x208)=0;
I(0x20C)=0;
I(0x210)=0;
I(0x214)=0;
I(0x218)=0;
I(0x21C)=0;
I(0x220)=0;
I(0x24C)=0;
I(0x250)=0;
I(0x254)=0;
I(0x258)=0;
H(0x25C)=0;
H(0x25E)=0;
H(0x260)=0;
H(0x262)=0;
H(0x264)=0;
H(0x266)=0;
F(0x224)=0.5f;
F(0x228)=one;
F(0x22C)=one;
F(0x230)=one;
F(0x234)=one;
F(0x238)=one;
F(0x23C)=one;
F(0x240)=one;
F(0x244)=one;
F(0x248)=one;
(*(unsigned long *)(p+0x268)) &= 0xFFFEFFFFFFFFFFFFUL;
(*(unsigned long *)(p+0x268)) &= 0xFFFDFFFFFFFFFFFFUL;
(*(unsigned long *)(p+0x268)) &= 0xFFFBFFFFFFFFFFFFUL;
}
