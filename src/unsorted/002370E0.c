/* UNIT 0x002370E0 0x00238728 .sdata=0x29DC38:0x29DC6C */
#include "gs_packet.h"
#include "include_asm.h"

/* One TU: every function here reads D_0029DC38 etc. $gp-relatively outside delay slots,
   so they must share the file that defines them. */

/* 16-byte packet tag; tag count = pointer difference (sra 4). */
typedef GsPacket Tag;

Tag *D_0029DC38 = 0; /* write cursor */
Tag *D_0029DC3C = 0;
Tag *D_0029DC40 = 0;
int D_0029DC44 = 0;
int D_0029DC48 = 0;
Tag *D_0029DC4C = 0;
Tag *D_0029DC50 = 0;
Tag *D_0029DC54 = 0;
int D_0029DC58 = 0x2000;
int D_0029DC5C = 0x800;
int D_0029DC60 = 0;
int D_0029DC64 = 0;
int D_0029DC68 = 10;



void func_002370E0(Tag *value)
{
    D_0029DC38 = value;
    D_0029DC40 = 0;
}

void func_002370F0(int x0)
{
    Tag *p = D_0029DC38;

    p->x0 = x0;
    p->x8 = 0x11000000;
    p->x4 = 0;
    p->xC = 0;
    D_0029DC38 = p + 1;
    D_0029DC50 = p;
    D_0029DC4C = p;
    D_0029DC40 = 0;
}

void func_00237120(void)
{
    Tag *p = D_0029DC38;

    if (p == D_0029DC4C + 1) {
        D_0029DC54 = D_0029DC4C;
    } else {
        p->x0 = 0;
        p->x4 = 0;
        p->x8 = 0;
        D_0029DC38 = p + 1;
        D_0029DC54 = p;
    }
    D_0029DC40 = 0;
}

void func_00237158(int a, int b, int c, int mode, int e, int f)
{
    if (D_0029DC40 != 0) {
        D_0029DC40->x0 &= 0x7FFF;
    }
    D_0029DC48 = mode;
    if (mode == 2) {
        func_00224660((GsPacket *)(D_0029DC38), 0, 0, 0, 2, 0, 0, 1, c);
        D_0029DC3C = 0;
        D_0029DC40 = 0;
    } else {
        func_00224660((GsPacket *)(D_0029DC38), a, b, c, mode, e, f, 1, 0);
        D_0029DC3C = D_0029DC38;
        D_0029DC40 = D_0029DC38;
        D_0029DC44 = c;
    }
    D_0029DC38++;
}

void func_00237220(void)
{
    Tag *p = D_0029DC3C;
    int flags = p->x0;
    int n = D_0029DC38 - p - 1;
    int q;

    if (D_0029DC48 != 0) {
        q = n * 2 / D_0029DC44;
    } else {
        q = n / D_0029DC44;
    }
    p->x0 = flags | q;
    D_0029DC3C = 0;
}

void func_00237278(void)
{
    Tag *p = D_0029DC54;

    p->xC = (D_0029DC38 - p - 1) | 0x50000000;
    D_0029DC54 = 0;
    D_0029DC40 = 0;
}

void func_002372A8(void)
{
    D_0029DC4C->x0 |= D_0029DC38 - D_0029DC4C - 1;
    D_0029DC4C = 0;
}

void func_002372D0(Tag *p)
{
    D_0029DC38 = p;
}

Tag *func_002372D8(void)
{
    return D_0029DC38;
}

/* Writes a 4-register GS packet (FRAME_1, XYOFFSET_1, SCISSOR_1, ZBUF_1) for a
   width x height target. */
void func_002372E0(unsigned int addr, unsigned int width, unsigned int height, unsigned int psm)
{
    Tag *t = D_0029DC38;
    unsigned long *p = (unsigned long *)t;
    unsigned int w64 = width >> 6;
    unsigned int fbw = 1;

    if (fbw < w64) {
        fbw = w64;
    }
    p[0] = (unsigned long)(addr >> 11) | ((unsigned long)fbw << 16) | ((unsigned long)psm << 24);
    p[1] = 0x4C;
    p[2] = (unsigned long)((0x800 - (width >> 1)) << 4) | ((unsigned long)((0x800 - (height >> 1)) << 4) << 32);
    p[3] = 0x18;
    p[4] = ((unsigned long)(width - 1) << 16) | ((unsigned long)(height - 1) << 48);
    p[5] = 0x40;
    p[6] = 0x131000000;
    p[7] = 0x4E;
    D_0029DC38 = t + 4;
}

/* sceGs register layouts (as in Sony's libgraph.h). */
#define GS_SET_TEX1(lcm, mxl, mmag, mmin, mtba, l, k)     ((unsigned long)(lcm) | ((unsigned long)(mxl) << 2) | ((unsigned long)(mmag) << 5)      | ((unsigned long)(mmin) << 6) | ((unsigned long)(mtba) << 9) | ((unsigned long)(l) << 19)      | ((unsigned long)(k) << 32))
#define GS_SET_TEX0(tbp, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld)     ((unsigned long)(tbp) | ((unsigned long)(tbw) << 14) | ((unsigned long)(psm) << 20)      | ((unsigned long)(tw) << 26) | ((unsigned long)(th) << 30) | ((unsigned long)(tcc) << 34)      | ((unsigned long)(tfx) << 35) | ((unsigned long)(cbp) << 37) | ((unsigned long)(cpsm) << 51)      | ((unsigned long)(csm) << 55) | ((unsigned long)(csa) << 56) | ((unsigned long)(cld) << 61))

static inline int Log2(unsigned int n)
{
    int bits = -1;

    while (n != 0) {
        n >>= 1;
        bits++;
    }
    return bits;
}

/* Writes TEX1_1 (0x14) and TEX0_1 (0x06) for a width x height texture (power-of-two sizes). */
void func_002373A0(unsigned int tbp, unsigned int width, unsigned int height, unsigned int psm,
                   unsigned int tfx, long filter)
{
    unsigned long *p;
    unsigned long *q;
    int tw;
    int th;

    p = (unsigned long *)D_0029DC38;
    p[0] = GS_SET_TEX1(0, 0, filter, filter, 0, 0, 0);
    p[1] = 0x14;
    tw = Log2(width);
    th = Log2(height);
    q = (unsigned long *)D_0029DC38;
    q[2] = GS_SET_TEX0(tbp >> 6, width >> 6, psm, tw, th, 1, tfx, 0, 0, 0, 0, 0);
    q[3] = 6;
    D_0029DC38 = (Tag *)(q + 4);
}

void func_00237480(unsigned int value)
{
    unsigned long *p = (unsigned long *)D_0029DC38;

    p[0] = 0x60;
    p[1] = 0x14;
    p[2] = 0x33001;
    p[3] = 0x47;
    p[4] = value;
    p[5] = 0x42;
    D_0029DC38 = (Tag *)(p + 6);
}


/* Draws a w x h sprite into the target, then a textured 2x-height copy offset by 8. */
void func_002374C8(unsigned int addr, unsigned int width, unsigned int height)
{
    int x0;
    int y0;

    {
        Tag *t = D_0029DC38;
        long *p = (long *)t;

        p[0] = 0x4000000044;
        p[1] = 0x42;
        D_0029DC38 = t + 1;
    }
    func_00237220();
    func_00237158(0, 0x441, 3, 0, 0x46, 1);
    {
        Tag *t = D_0029DC38;

        x0 = (0x800 - (width >> 1)) << 4;
        y0 = (0x800 - (height >> 1)) << 4;
        t[0].x0 = 0;
        t[0].x4 = 0;
        t[0].x8 = 0;
        t[0].xC = 0x40;
        t[1].x0 = x0;
        t[1].x4 = y0;
        t[1].x8 = 0xFFFFFF;
        t[2].x0 = x0 + (width << 4);
        t[2].x4 = y0 + (height << 4);
        t[2].x8 = 0xFFFFFF;
        D_0029DC38 = t + 3;
    }
    func_00237220();
    func_00237158(0, 0xE, 1, 0, 0, 0);
    func_002372E0(addr, width, height * 2, 2);
    func_002373A0(addr, width, height * 2, 2, 1, 0);
    {
        Tag *t = D_0029DC38;
        long *p = (long *)t;

        p[0] = 0x30000;
        p[1] = 0x47;
        p[3] = 0x3F;
        D_0029DC38 = t + 2;
    }
    func_00237220();
    func_00237158(0, 0x4343, 4, 0, 0x116, 1);
    {
        Tag *t = D_0029DC38;

        x0 += 0x80;
        y0 = (0x800 - height) << 4;
        t[0].x0 = 8;
        t[0].x4 = 8;
        t[1].x0 = x0;
        t[1].x4 = y0;
        t[1].x8 = 0xFFFFFF;
        t[2].x0 = (width << 4) + 8;
        t[2].x4 = (height << 5) + 8;
        t[3].x0 = x0 + (width << 4);
        t[3].x4 = y0 + (height << 5);
        t[3].x8 = 0xFFFFFF;
        D_0029DC38 = t + 4;
    }
}

/* Writes a fixed 4-tag (64-byte) GS packet at the cursor. */
void func_002376C8(void)
{
    Tag *t = D_0029DC38;
    long *p = (long *)t;

    p[0] = 0x7000D;
    p[1] = 0x47;
    p[2] = 0x61;
    p[3] = 0x14;
    p[4] = 0x131000000;
    p[5] = 0x4E;
    p[7] = 8;
    p[6] = 0;
    D_0029DC38 = t + 4;
}

void func_00237480(unsigned int value);

void func_00237720(Tag *buffer, unsigned int addr, unsigned int size, int arg3, int flag)
{
    func_002370E0(buffer);
    func_002370F0(0x60000000);
    func_00237120();
    func_00237158(0, 0xE, 1, 0, 0, 0);
    func_002372E0(addr, size, size, 0);
    func_00237480(arg3);
    func_00237220();
    if (flag != 0) {
        func_00237278();
    }
    func_002372D8();
}
Tag *func_002377D0(Tag *start,int addr,int size,int finish) {
Tag *cursor;
func_002372D0(start);
func_00237120();
func_00237158(0,14,1,0,0,0);
func_002374C8(addr,size,size);
func_00237220();
func_00237158(0,14,1,0,0,0);
cursor=D_0029DC38;
*(long *)((char *)cursor+8)=0x3f;
D_0029DC38=cursor+1;
func_002372E0(addr,size,size,0);
func_00237480(0x44);
func_00237220();
if(finish) func_00237278();
return func_002372D8();
}
void func_002378B8(Tag *p)
{
    func_002372D0(p);
    func_00237120();
    func_00237158(0, 14, 1, 0, 0, 0);
    func_002376C8();
    func_00237220();
    func_00237278();
    func_002372A8();
}

struct Entry { int x0, x4, x8; short xC, xE; };
extern struct Entry D_0067DF90[];
extern char D_29C0[];
typedef struct { int x0; int x4; int x8; int xC; int x10; int x14; int x18; int x1C; int x20; int x24; int x28; int x2C; int x30; float x34; int x38; int x3C; int x40; int x44; int x48; int x4C; int x50; int x54; int x58; int x5C; int x60; int x64; int x68; int x6C; int x70; int x74; int x78; int x7C; int x80; int x84; int x88; int x8C; int x90; int x94; int x98; int x9C; int xA0; int xA4; int xA8; int xAC; int xB0; int xB4; int xB8; int xBC; int xC0; int xC4; int xC8; int xCC; int xD0; int xD4; int xD8; int xDC; int xE0; int xE4; int xE8; int xEC; int xF0; int xF4; int xF8; int xFC; int x100; int x104; int x108; int x10C; int x110; } Packet;

#define B(p,n) ((Packet *)((char *)(p)+(n)))
#define W(p,n) ((p)->w[(n)/4])
#define H(p,n) (*(short *)((char *)(p)+(n)))
#define F(p,n) (*(float *)((char *)(p)+(n)))
#define L(p,n) (*(long *)((char *)(p)+(n)))
#define PACKET_CURSOR ((Packet *)D_0029DC38)
static inline float ToFloat(int value) { return value; }
void func_00237918(int a0,int a1,int a2,int first,int second,int size,Packet **out1,Packet **out2,Packet **out3) {
    int secondOffset=second*16;
    struct Entry *entry;
    int half,quarter,right,left,i,j,x0,x1,y0,y1,nextY0,nextY1;
    Packet *p0,*p1,*p2,*p3,*p4,*p5,*p6,*end;
    func_00237720((Tag *)a0,a1,a2,72,0);
    func_00237158(0,14,1,0,0,0);
    entry=&D_0067DF90[first];
    p0=PACKET_CURSOR;
    p0->x0=entry->x0;
    p0->x4=entry->x4;
    L(p0,8)=6;
    D_0029DC38=(Tag *)B(p0,16);
    func_00237220();
    func_00237278();
    size>>=1; half=size;
    PACKET_CURSOR->x0=0x6D100008;
    y0=8; y1=half-8; i=0;
    do {
        x0=8; x1=half-8; nextY0=y1+16; nextY1=y1+half; j=1;
        do {
            p1=PACKET_CURSOR;
            H(p1,4)=x0; H(p1,12)=x0; H(p1,24)=x0; H(p1,32)=x0;
            H(p1,8)=x1; H(p1,16)=x1; H(p1,20)=x1; H(p1,28)=x1;
            H(p1,6)=y0; H(p1,10)=y1; H(p1,14)=y1; H(p1,18)=y0;
            H(p1,22)=y0; H(p1,26)=y1; H(p1,30)=y1; H(p1,34)=y0;
            x0=x1+16; x1+=half;
            D_0029DC38=(Tag *)B(p1,32);
        } while(--j>=0);
        y0=nextY0; y1=nextY1; i++;
    } while(i<2);
    end=B(p1,32);
    end->x4=0x6C070018;
    func_00224660((GsPacket *)((Tag *)B(p1,40)),0,0x441,3,0,6,1,0,1);
    quarter=a2>>2; left=(2048-a2)*16; right=(a2+2048)*16;
    p2=PACKET_CURSOR;
    p2->x18=0; p2->x1C=0; p2->x20=0; p2->x24=0; p2->x44=0;
    p2->x28=left; p2->x2C=left; p2->x30=0xFFFFFF; p2->x40=0xFFFFFF;
    p2->x34=ToFloat(quarter);
    p2->x38=right; p2->x3C=right;
    func_00224660((GsPacket *)((Tag *)B(p2,72)),0x1A44F204,0x43431,5,0,0x156,1,0,0x168);
    *out1=B(PACKET_CURSOR,88);
    p3=PACKET_CURSOR;
    p3->x7C=((unsigned int)D_29C0>>3)|0x14000000;
    p3->x78=0x10000000; p3->x80=0x10000000; p3->x84=0x6C080008;
    *out2=B(PACKET_CURSOR,136); *out3=B(PACKET_CURSOR,184);
    p4=PACKET_CURSOR;
    p4->x10C=0x11000000; p4->x108=0;
    D_0029DC38=func_002377D0((Tag *)B(p4,272),a1,a2,0);
    func_00237158(0,14,1,0,0,0);
    entry=(struct Entry *)((char *)D_0067DF90+secondOffset);
    p5=PACKET_CURSOR; p5->x0=entry->x0; p5->x4=entry->x4; L(p5,8)=6;
    D_0029DC38=(Tag *)B(p5,16);
    func_00237220();
    func_00237278();
    p6=PACKET_CURSOR;
    p6->x0=0x11000000; p6->x4=0x17000000; p6->x8=0; p6->xC=0x11000000;
    D_0029DC38=(Tag *)B(p6,16); ((void (*)(Tag *, int, int))func_002378B8)(D_0029DC38,a1,a2);
}
#undef PACKET_CURSOR


void func_00237C78(int index, int width, unsigned int height) {
    long bits;
    unsigned int n = height;
    int count = -1;
    if (height) { do { n >>= 1; count++; } while (n); }
    bits = (width >> 6) | ((long)((int)height >> 6) << 14) | ((long)count << 26) | ((long)count << 30) | ((long)0x8000 << 19);
    D_0067DF90[index].x0 = ((volatile int *)&bits)[0];
    D_0067DF90[index].x4 = ((volatile int *)&bits)[1];
    D_0067DF90[index].x8 = -1;
    D_0067DF90[index].xC = height * 16;
    D_0067DF90[index].xE = height * 16;
}

extern Tag D_00759790[2][0x78];
extern Tag *D_0029E3A8[2];
extern Tag *D_0029E3B0[2];
extern Tag *D_0029E3B8[2];
extern Tag *D_0029E3C0[2];
extern Tag *D_0029E3C8[2];

/* Textured sprite quad: UV (0,0)-(0x400,0x400), XY centred on 0x8000 with half-size n. */
#define QUAD(n) \
{ \
    Tag *t = D_0029DC38; \
 \
    t[0].x0 = 0; \
    t[0].x4 = 0; \
    t[0].x8 = 0x3F800000; \
    t[1].x0 = 0x8000 - n; \
    t[1].x4 = 0x8000 - n; \
    t[1].x8 = 0xFFFFFF; \
    t[2].x0 = 0x400; \
    t[2].x4 = 0x400; \
    t[2].x8 = 0x3F800000; \
    t[3].x0 = n + 0x8000; \
    t[3].x4 = n + 0x8000; \
    t[3].x8 = 0xFFFFFF; \
    D_0029DC38 = t + 4; \
}

void func_00237D20(int width, int height)
{
    float scale = 1.07f;
    int i;
    int j;
    int n;

    D_0029DC58 = width << 4;
    D_0029DC5C = height << 4;
    for (i = 0; i < 2; i++) {
        func_002370E0(D_00759790[i]);
        func_002370F0(0x60000000);
        func_00237120();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            p[0] = i ? 0x1013F8001013F00 : 0x1013F8001013F40;
            p[1] = 0x50;
            p[2] = 0;
            p[3] = 0x51;
            p[4] = 0x4000000040;
            p[5] = 0x52;
            p[6] = 2;
            p[7] = 0x53;
            D_0029DC38 = t + 4;
        }
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        func_002373A0(0, width, height, 1, 1, 1);
        D_0029E3A8[i] = D_0029DC38 - 1;
        func_002372E0(0xFF000, 0x40, 0x40, 1);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            p[0] = 0x33001;
            p[1] = 0x47;
            D_0029DC38 = t + 1;
        }
        func_00237220();
        func_00237158(0, 0x551, 3, 0, 6, 1);
        {
            Tag *t = D_0029DC38;

            t[0].x0 = 0;
            t[0].x4 = 0;
            t[0].x8 = 0;
            t[0].xC = 0;
            t[1].x0 = 0x7E00;
            t[1].x4 = 0x7E00;
            t[1].x8 = 0xFFFFFF;
            t[2].x0 = 0x8200;
            t[2].x4 = 0x8200;
            t[2].x8 = 0xFFFFFF;
            D_0029DC38 = t + 3;
        }
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x116, 1);
        {
            Tag *t = D_0029DC38;

            D_0029E3B0[i] = t;
            t[0].x8 = 0x3F800000;
            t[1].x8 = 0xFFFFFF;
            t[2].x8 = 0x3F800000;
            t[3].x8 = 0xFFFFFF;
            D_0029DC38 = t + 4;
        }
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            p[0] = i ? 0x1013F0001013FC0 : 0x1013F4001013FC0;
            p[1] = 0x50;
            p[2] = 0;
            p[3] = 0x51;
            p[4] = 0x4000000040;
            p[5] = 0x52;
            p[6] = 2;
            p[7] = 0x53;
            D_0029DC38 = t + 4;
        }
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            p[0] = 0x4000000064;
            p[1] = 0x42;
            D_0029DC38 = t + 1;
        }
        func_002373A0(0xFE000, 0x40, 0x40, 1, 1, 0);
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x156, 1);
        QUAD(0x200);
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            D_0029E3B8[i] = t;
            p[1] = 6;
            p[2] = 0x8000000062;
            p[3] = 0x42;
            p[4] = 1;
            p[5] = 0x3F;
            D_0029DC38 = t + 3;
        }
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x156, 1);
        QUAD(0x200);
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        func_002373A0(0xFF000, 0x40, 0x40, 1, 1, 0);
        func_002372E0(0xFE000, 0x40, 0x40, 1);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            p[0] = 0x8000000068;
            p[1] = 0x42;
            p[2] = 1;
            p[3] = 0x3F;
            D_0029DC38 = t + 2;
        }
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x116, 1);
        QUAD(0x200);
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        func_002373A0(0xFF000, 0x40, 0x40, 1, 1, 1);
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x156, 1);
        for (j = 0; j < 3; j++) {
            n = (int)(scale * 512.0f);
            QUAD(n);
            scale *= 1.07f;
        }
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        func_002373A0(0xFE000, 0x40, 0x40, 1, 1, 0);
        func_002372E0(0xFF000, 0x40, 0x40, 1);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            p[0] = 0x8000000068;
            p[1] = 0x42;
            p[2] = 1;
            p[3] = 0x3F;
            D_0029DC38 = t + 2;
        }
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x116, 1);
        QUAD(0x200);
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        func_002373A0(0xFE000, 0x40, 0x40, 1, 1, 1);
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x156, 1);
        n = (int)(scale * 512.0f);
        QUAD(n);
        n = (int)(512.0f / scale);
        QUAD(n);
        n = (int)(512.0f / (scale * scale));
        QUAD(n);
        func_00237220();
        func_00237158(0, 0xE, 1, 0, 0, 0);
        func_002373A0(0xFF000, 0x40, 0x40, 1, 1, 1);
        func_002372E0(0, width, height, 1);
        {
            Tag *t = D_0029DC38;
            long *p = (long *)t;

            D_0029E3C0[i] = t - 4;
            p[0] = 0x2000000068;
            p[1] = 0x42;
            D_0029DC38 = t + 1;
        }
        func_00237220();
        func_00237158(0, 0x5353, 4, 0, 0x156, 1);
        {
            Tag *t = D_0029DC38;

            D_0029E3C8[i] = t;
            t[0].x0 = 0;
            t[0].x4 = 0;
            t[0].x8 = 0x3F800000;
            t[1].x8 = 0xFFFFFF;
            t[2].x0 = 0x400;
            t[2].x4 = 0x400;
            t[2].x8 = 0x3F800000;
            t[3].x8 = 0xFFFFFF;
            D_0029DC38 = t + 4;
        }
        func_00237220();
        func_00237278();
        func_002372A8();
    }
    D_0029DC60 = scale * (scale * 1024.0f);
}
