/* UNIT 0x0010A7B0 0x0010AD50 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { int x0, x4; float x8; int xC; } Vec;
#define I(o) (*(int *)(p + (o)))
#define F(o) (*(float *)(p + (o)))
#define Z(o) __asm__ volatile("sqc2 $vf0, %0" : "=m"(*(u128 *)(p + (o))))
#define ZP(q) __asm__ volatile("sqc2 $vf0, %0" : "=m"(*(u128 *)(q)))
#define L(o) (*(unsigned long *)(p + (o)))

extern void func_00254204(void *dst, int value, int size);
extern void func_00118EB0(void *unused, void *obj);
extern void func_00221F20(void *out);
extern char D_0029EA20[];

void func_0010A7B0(char *p)
{
    Vec *c, *d;
    int *e, *g;
    u128 *q;
    int i;
    float one;

    func_00254204(p, 0, 0x450);
    I(0) = 0; I(4) = 0; I(8) = 0;
    func_00118EB0(D_0029EA20, p + 0x10);
    func_00118EB0(D_0029EA20, p + 0x30);
    func_00118EB0(D_0029EA20, p + 0x50);
    I(0x80) = 0; I(0x7C) = 0; I(0x78) = 0; I(0x74) = 0; I(0x70) = 0;
    func_00221F20(p + 0x90);
    Z(0xD0); Z(0xE0); Z(0xF0); Z(0x100);
    q = (u128 *)(p + 0x110); i = 3;
    do { ZP(q); i--; q++; } while (i >= 0);
    c = (Vec *)(p + 0x200); d = (Vec *)(p + 0x210);
    e = (int *)(p + 0x3C4); g = (int *)(p + 0x3D4);
    q = (u128 *)(p + 0x150); i = 8;
    do { ZP(q); i--; q++; } while (i >= 0);
    Z(0x1E0); Z(0x1F0);
    one = 1.0f;
    I(0x200) = 0; c->x4 = 0; c->x8 = one; I(0x210) = 0; d->x4 = 0; d->x8 = 0;
    Z(0x220); Z(0x230);
    I(0x240) = 0;
    I(0x244) = 0; I(0x248) = 0; I(0x24C) = 0; I(0x250) = 0; I(0x254) = 0;
    I(0x258) = 0; I(0x25C) = 0; I(0x260) = 0; I(0x264) = 0; I(0x268) = 0;
    I(0x26C) = 0; I(0x270) = 0; I(0x274) = 0; I(0x278) = 0; I(0x27C) = 0;
    I(0x280) = 0; I(0x284) = 0; I(0x288) = 0; I(0x28C) = 0; I(0x290) = 0;
    I(0x294) = 0; I(0x298) = 0; I(0x29C) = 0; I(0x2A0) = 0; I(0x2A4) = 0;
    I(0x2A8) = 0;
    F(0x2AC) = one; F(0x2B0) = one;
    I(0x2B4) = 0; I(0x2B8) = 0; I(0x2BC) = 0; I(0x2C0) = 0; I(0x2C4) = 0;
    I(0x2C8) = 0; I(0x2CC) = 0; I(0x2D0) = 0;
    I(0x3A4) = 0; I(0x3A8) = 0; I(0x3AC) = 0; I(0x3B0) = 0; I(0x3B4) = 0;
    I(0x3B8) = 0; I(0x3BC) = 0; I(0x3C0) = 0;
    i = 3; do { e[i] = 0; i--; } while (i >= 0);
    i = 8; do { g[i] = 0; i--; } while (i >= 0);
    I(0x440) = 0;
    I(0x414) = 0xB4; I(0x424) = 0xF;
    L(0x440) &= ~(1UL << 32);
    L(0x440) &= ~(1UL << 33);
    L(0x440) &= ~(1UL << 34);
    L(0x440) &= ~(1UL << 35);
    L(0x440) &= ~(1UL << 36);
    L(0x440) &= ~(1UL << 37);
    L(0x440) &= ~(1UL << 38);
    L(0x440) &= ~(1UL << 39);
    L(0x440) &= ~(1UL << 40);
    L(0x440) &= ~(1UL << 41);
    L(0x440) &= ~(1UL << 43);
    L(0x440) &= ~(1UL << 42);
    L(0x440) &= ~(1UL << 44);
    L(0x440) &= ~(1UL << 46);
    L(0x440) &= ~(1UL << 47);
    L(0x440) &= ~(1UL << 48);
    L(0x440) &= ~(1UL << 49);
    L(0x440) &= ~(1UL << 50);
    L(0x440) &= ~(1UL << 51);
    L(0x440) &= ~(1UL << 52);
    L(0x440) &= ~(1UL << 53);
    L(0x440) &= ~(1UL << 54);
    L(0x440) &= ~(1UL << 55);
    L(0x440) &= ~(1UL << 56);
    L(0x440) &= ~(1UL << 57);
    L(0x440) &= ~(1UL << 60);
    L(0x440) &= ~(1UL << 61);
    L(0x440) &= ~(1UL << 62);
    L(0x440) &= ~(1UL << 63);
    I(0x3F8) = 0; I(0x3FC) = 0; I(0x400) = 0; I(0x404) = 0; I(0x408) = 0;
    I(0x40C) = 0; I(0x410) = 0; I(0x418) = 0; I(0x41C) = 0; I(0x420) = 0;
    I(0x428) = 0; I(0x42C) = 0; I(0x430) = 0; I(0x434) = 0; I(0x438) = 0;
    I(0x43C) = 0;
    L(0x448) &= ~1UL;
    L(0x448) &= ~2UL;
    L(0x448) &= ~4UL;
    L(0x448) &= ~8UL;
    L(0x448) &= ~16UL;
    L(0x448) &= ~32UL;
}
