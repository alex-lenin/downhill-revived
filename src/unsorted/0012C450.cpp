/* UNIT 0x0012C450 0x0012C668 */
typedef unsigned int u128 __attribute__((mode(TI)));

class Part {
public:
    int x0;
    virtual void v0();
    virtual void v1();
    virtual void reset();
};

struct Sub {
    char pad[0x16D4];
    void *x16D4;
};

struct Placement {
    u128 pos;
    u128 matrix[4];
};

class Rider {
public:
    char pad[0x1E40];
    Sub x1E40;
    void func_0012C450();
};

extern "C" {
extern char D_0029EA20[], D_0032FC60[], D_0043EEC0[];
void func_0017D740(void *, void *);
int func_001A0498(void *, u128 *, u128 *);
u128 *func_001177F8(void *, int);
void func_0010AD50(void *, u128 *, u128 *, int);
void func_00231A78(void *, u128 *);
void func_00231A08(void *, u128 *);
void func_00134FB0(void *);
void func_0012DDC0(void *);
void func_001AC418(void *);
void func_0012C700(void *);
void func_00158370(void *);
void func_002211F0(void *, void *);
void func_00138B40(void *, void *);
}

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

#define I(o) (*(int *)(p + (o)))
#define H(o) (*(short *)(p + (o)))
#define B(o) (*(char *)(p + (o)))
#define F(o) (*(float *)(p + (o)))

static inline void SetBody(char *body, u128 *pos, u128 *matrix)
{
    func_00231A78(body, matrix);
    func_00231A08(body, pos);
}

void Rider::func_0012C450()
{
    char *p = (char *)this;
    Placement start;
    char *sub;
    Sub *saved;
    Part **parts;
    int count;
    char *x40A0;
    char *x7A80;
    Part *x5FF0;

    {
        int found = 0;

        func_0017D740(p + 0xFE0, p);
        if (func_001A0498(D_0043EEC0, &start.pos, start.matrix)) {
            found = 1;
        } else {
            u128 *v = func_001177F8(D_0029EA20, I(0x7978));
            COPY_VECTOR(start.pos, v);
        }
        if (*(void **)(p + 0x78D4)) {
            func_0010AD50(*(void **)(p + 0x78D4), &start.pos, start.matrix, found);
        } else {
            SetBody(p + 0x40C0, &start.pos, start.matrix);
        }
    }
    func_00134FB0(p + 0x40C0);
    sub = p + 0x1E40;
    func_0012DDC0(p);
    x40A0 = p + 0x40A0;
    x7A80 = p + 0x7A80;
    x5FF0 = (Part *)(p + 0x5FF0);
    parts = (Part **)(p + 0x6394);
    count = 17;
    I(0x798C) = 0;
    I(0x7990) = 0;
    I(0x7994) = 0;
    I(0x7A3C) = 0;
    B(0x7A68) = 0;
    I(0x7ADC) = 0;
    I(0x7998) = 0;
    I(0x799C) = 0;
    I(0x79AC) = 0;
    I(0x7A78) = 0;
    H(0x7966) = 0;
    B(0x7A69) = 0;
    H(0x7964) = 0;
    H(0x7968) = 0;
    I(0x7A74) = 0;
    B(0x7A6A) = 0;
    I(0x7A6C) = 0;
    I(0x7B00) = 0;
    F(0x7BA0) = 1.0f;
    func_001AC418(sub);
    func_0012C700(p);
    /* a separate copy of sub, assigned here so it gets its own register */
    saved = &x1E40;
    do {
        Part *part = *parts;
        --count;
        ++parts;
        part->reset();
    } while (count >= 0);
    func_00158370(x40A0);
    func_002211F0(saved->x16D4, x7A80);
    I(0x7A54) = 0;
    I(0x7A58) = 0;
    I(0x7AC4) = 0;
    func_00138B40(D_0032FC60, p);
    x5FF0->reset();
    B(0x7BA9) = 0;
    B(0x7BAE) = 0;
    B(0x7BAC) = 0;
    B(0x7BAA) = 0;
    B(0x7BA8) = 0;
    B(0x7BAB) = 0;
    B(0x7BAD) = 0;
    B(0x7BAF) = 0;
    B(0x7BB0) = 0;
    B(0x7BB1) = 0;
    B(0x7BB3) = 0;
    B(0x7BB6) = 0;
    I(0x7BB8) = 0;
    I(0x7BC0) = 0;
    I(0x7BC4) = 0;
    I(0x7BC8) = 0;
}
