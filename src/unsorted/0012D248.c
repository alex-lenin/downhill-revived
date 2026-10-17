/* UNIT 0x0012D248 0x0012D300 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Body {
    char pad0[0x10];
    u128 x10;
    char pad20[0x70];
    u128 x90;
    u128 xA0;
} Body;

typedef struct Sub {
    char pad0[0x50];
    u128 x50;
    char pad60[0x4A4 - 0x60];
    short x4A4;
} Sub;

typedef struct Rider {
    char pad0[0x40C0];
    Sub x40C0;
    char pad[0x792C - 0x40C0 - sizeof(Sub)];
    Body *x792C;
    char pad7930[0x7AB0 - 0x7930];
    u128 x7AB0;
    char pad7AC0[0x7B90 - 0x7AC0];
    unsigned char x7B90;
    char pad7B91[3];
    int x7B94;
    char pad7B98[4];
    unsigned char x7B9C;
    unsigned char x7B9D;
} Rider;

extern void func_0012F7D0(Rider *r);
extern void func_0012F8D8(Rider *r);
extern void func_0012F6A0(Rider *r);
extern void func_0012FA70(Rider *r);
extern void func_001306C0(Rider *r);
extern int func_00138560(Sub *s);

void func_0012D248(Rider *r)
{
    Sub *sub = &r->x40C0;
    int on = 1;

    func_0012F7D0(r);
    func_0012F8D8(r);
    func_0012F6A0(r);
    func_0012FA70(r);
    func_001306C0(r);
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(r->x7AB0) : "r"(&sub->x50) : "$2");
    r->x792C->xA0 = r->x792C->x90;
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(r->x792C->x90) : "r"(&r->x792C->x10) : "$2");
    r->x7B90 = 0;
    r->x7B94 = 0;
    if ((unsigned char)func_00138560(sub)) {
        r->x7B9C = on;
        if (sub->x4A4 > 40) {
            r->x7B9D = on;
        }
    }
}
