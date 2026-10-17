/* UNIT 0x001A0730 0x001A0858 .rodata=0x28E5F0:0x28E608 */
typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    char pad0[4];
    u8 x4;
} Sub;

typedef struct {
    char pad0[0x510];
    Sub *x510;
    Sub *x514;
    u8 x518;
    s8 x519;
    s8 x51A;
    u8 x51B;
    void *x51C;
} Obj;

extern char *D_003F5404;
extern char D_004B25A0[];

extern void func_001A09A8();
extern void func_001A1380();
extern void func_001A1990();
extern void func_001A1E88();
extern void func_0018DC20();
extern void func_001BB040();
extern void func_001BAF10();

void func_001A0730(Obj *this)
{
    static const void *table[5] __attribute__((section(".rdata"), aligned(8))) = {
        &&case0, &&case1, &&case2, &&case3, &&case4
    };
    Sub *p;
    s8 tmp;
    int index;

    p = this->x510;
    if (p != 0) {
        p->x4 = 1;
    }
    p = this->x514;
    if (p != 0) {
        p->x4 = 1;
    }
    this->x51B = 0;
    if (D_003F5404 != 0) {
        D_003F5404[4] = 0;
    }

    index = (signed char)(this->x518 - 1);
    if ((unsigned int)index >= 5) {
        goto end;
    }
    goto *table[index];

case0:
    func_001A09A8(this, this->x51C);
    goto end;
case1:
    this->x519 = 0;
    func_001A1380(this, this->x51C, 0xD);
    goto end;
case2:
    tmp = this->x519;
    func_001A1380(this, this->x51C, this->x51A);
    this->x519 = tmp;
    goto end;
case3:
    func_001A1990(this, this->x51C);
    goto end;
case4:
    func_001A1E88(this, this->x51C);
end:
    func_0018DC20(this);
    func_001BB040(D_004B25A0, 3);
    func_001BAF10(D_004B25A0, 4);
}
