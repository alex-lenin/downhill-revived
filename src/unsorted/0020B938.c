/* UNIT 0x0020B938 0x0020C0A0 .sdata=0x29D9E0:0x29DA10 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0020BD18_30_func_0020BD18 func_0020BD18
#else
#define CallScalarBridge0020BD18_30_func_0020BD18 ScalarBridge0020BD18_30
#endif
#include "include_asm.h"
typedef struct { char pad[0x10]; int x10; char pad14[0x14]; int x28, x2C, x30, x34; } Obj;
volatile int D_0029D9E0 = 0;
volatile int D_0029D9E4 = 0;
volatile int D_0029D9E8 = 0;
volatile int D_0029D9EC = 0;
extern void func_00243680(Obj *, int, int, int, int, int);
void func_0020B938(Obj *obj, int width, int height) {
 int roundedHeight = height + 31;
 int roundedWidth = width + 63;
 D_0029D9E0 = 0;
 D_0029D9E4 = 0;
 D_0029D9E8 = 0;
 D_0029D9EC = 0;
 obj->x28 = 0;
 obj->x2C = ((roundedWidth / 64) * ((roundedHeight / 32) << 11)) >> 11;
 obj->x30 = width;
 obj->x34 = height;
 func_00243680(obj, 0, 640, 448, 0, 0);
 obj->x10 = (obj->x10 & ~0x7E00) | 0x1400;
}

int D_0029D9F0 = 0;
int D_0029D9F4 = 0;
extern void *func_0020BC08(unsigned long *, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void *func_0020BC60(int *, long, int, int, int, int, int, int);
extern void *func_0020BCE8(void *,long,long,long);

#if defined(__mips__)
extern void *func_0020BD18(void *,long,long,long);
#else

#endif

extern void *func_0020BD48(void *,long,long);
extern void *func_0020BD78(void *,unsigned int);
void func_0020B9E8(int unused, unsigned int address, unsigned int source, int skip, int u4,int u5,int u6,int u7, int width, int height) {
 int rows=width>>4;
 int cols=height>>4;
 int row,col;
 int x,y;
 void *out=(void *)((address & 0x0FFFFFFF)|0x20000000);
 if(!skip) {
 x=(unsigned int)D_0029D9F0>>6;
 y=(unsigned int)D_0029D9F4>>6;
 row=0;
 out=func_0020BC08(out,0,0,0,1,0,3);
 out=func_0020BC60(out,14,1,0,0,0,0,2);
 out=func_0020BCE8(out,x,y,0);
 out=func_0020BD48(out,16,16);
 for(;row<rows;row++) {
 for(col=0;col<cols;col++) {
 out=func_0020BC08(out,0,0,0,1,0,4);
 out=func_0020BC60(out,14,1,0,0,0,0,2);
 out=((void *(*)(void *,int,int,int))CallScalarBridge0020BD18_30_func_0020BD18)(out,0,row<<4,col<<4);
 out=func_0020BD78(out,0);
 out=func_0020BC60(out,0,0,2,0,0,0,64);
 out=func_0020BC08(out,0,source & 0x0FFFFFFF,0,3,0,64);
 source+=0x400;
 }
 }
 }
 out=func_0020BC08(out,0,0,0,7,0,1);
 func_0020BC60(out,14,1,0,0,0,1,0);
}


void *func_0020BC08(unsigned long *out, unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e, unsigned int f) {
    *out = ((unsigned long)a << 63) | ((unsigned long)(b & 0xFFFFFFF0) << 32) | ((unsigned long)c << 31) | ((unsigned long)d << 28) | ((unsigned long)e << 26) | (unsigned long)f;
    return (char *)out + 0x10;
}

void *func_0020BC60(int *out, long value, int a, int b, int c, int d, int e, int f) {
    out[0] = (e << 15) | f;
    out[1] = (d << 14) | (c << 15) | (b << 26) | (a << 28);
    out[2] = (int)(value & 0xFFFFFFFFUL);
    out[3] = (int)(value >> 32);
    return out + 4;
}

typedef struct GifTag {
    int x0, x4;
    volatile int x8, xC;
} GifTag;
GifTag *func_0020BCB8(GifTag *obj, int value, long packed) {
    obj->x8 = value;
    obj->xC = 0;
    obj->x0 = (int)(packed & 0xFFFFFFFFUL);
    obj->x4 = (int)(packed >> 32);
    return obj + 1;
}

void *func_0020BCE8(void *out, long x, long y, long z) {
    return func_0020BCB8(out, 0x50, (z << 56) | (y << 48) | (x << 32));
}

void *func_0020BD18(void *out, long x, long y, long z) {
    return func_0020BCB8(out, 0x51, (x << 59) | (z << 48) | (y << 32));
}

void *func_0020BD48(void *out, long x, long y) {
    return func_0020BCB8(out, 0x52, (y << 32) | ((unsigned long)(x << 32) >> 32));
}

void *func_0020BD78(void *out, unsigned int c)
{
    return func_0020BCB8(out, 0x53, (unsigned long)c);
}

int D_0029D9F8 = 0;
int D_0029D9FC = 0;
volatile unsigned char D_0029DA00 = 0;

typedef struct Display {
    int x0;
    int x4;
    char pad8[0xB8 - 0x8];
} Display;
extern Display D_00663B58;

#define GS_PMODE    ((volatile unsigned long *)0x12000000)
#define GS_DISPFB1  ((volatile unsigned long *)0x12000070)
#define GS_DISPLAY1 ((volatile unsigned long *)0x12000080)
#define GS_DISPFB2  ((volatile unsigned long *)0x12000090)
#define GS_DISPLAY2 ((volatile unsigned long *)0x120000A0)
#define GS_SET_DISPLAY(dx, dy, magh, magv, dw, dh)     ((unsigned long)(dx) | ((unsigned long)(dy) << 12) | ((unsigned long)(magh) << 23) |      ((unsigned long)(magv) << 27) | ((unsigned long)(dw) << 32) | ((unsigned long)(dh) << 44))

void func_0020BDA0(int mode) {
    if (D_0029D9F8 != 0) {
        D_0029DA00 = 1;
    }
    if (D_0029DA00 != 0) {
        if (mode == 0) {
            *GS_PMODE = 0xFF42;
            *GS_DISPFB2 = 0x148C;
            *GS_DISPLAY2 = GS_SET_DISPLAY(636, 50, 3, 0, 2560, 447);
            *GS_DISPFB1 = 0x1518;
            *GS_DISPLAY1 = GS_SET_DISPLAY(636, 90, 3, 0, D_00663B58.x0 * 4, D_00663B58.x4 - 1);
        } else if (mode == 1) {
            *GS_PMODE = 0xFF03;
            *GS_DISPFB1 = 0x148C;
            *GS_DISPLAY1 = GS_SET_DISPLAY(636, 50, 3, 0, 2560, 447);
            *GS_DISPFB2 = 0x1518;
            *GS_DISPLAY2 = GS_SET_DISPLAY(636, 90, 3, 0, D_00663B58.x0 * 4, D_00663B58.x4 - 1);
        } else {
            *GS_PMODE = 0xFF02;
            *GS_DISPFB1 = 0x148C;
            *GS_DISPLAY1 = GS_SET_DISPLAY(636, 50, 3, 0, 2560, 447);
            *GS_DISPFB2 = 0x1518;
            *GS_DISPLAY2 = GS_SET_DISPLAY(636, 90, 3, 0, D_00663B58.x0 * 4, D_00663B58.x4 - 1);
        }
    }
}

typedef struct DoubleBuffer {
    int state;
    char pad4[0x40 - 0x4];
    char x40[0x1AB40 - 0x40];
    char x1AB40[4];
} DoubleBuffer;

typedef struct Ring {
    char pad0[0x8];
    int x8;
    char padC[0x10 - 0xC];
    int x10;
} Ring;

extern int D_0029E178;
extern Ring D_00663B40;
extern int func_002433A8(int, int);
extern DoubleBuffer *func_0023E100(Ring *);
extern void func_00224438(char *);
extern void func_0025EEA0(void);

int func_0020BED0(void) {
    DoubleBuffer *buffer;
    int swapped;

    if (D_0029D9FC == 0) {
        return 0;
    }
    swapped = 0;
    D_0029D9E8 = (*(volatile unsigned long *)0x12001000 >> 13) & 1;
    if (D_0029D9E0 != 0) {
        D_0029D9EC = func_002433A8(1, 0);
        if (D_0029D9EC == 0) {
            buffer = func_0023E100(&D_00663B40);
            if (buffer == 0) {
                goto end;
            }
            D_0029E178 = (D_00663B40.x8 + (D_00663B40.x10 - 1)) % D_00663B40.x10;
            if (D_0029D9E8 == 0 && buffer->state == 2) {
                func_00224438(buffer->x1AB40);
                func_002433A8(0, 0);
                buffer->state = 1;
                swapped = 1;
            } else if (D_0029D9E8 != 0 && buffer->state == 1) {
                func_00224438(buffer->x40);
                func_002433A8(0, 0);
                buffer->state = 0;
                D_0029D9E4 = 1;
                swapped = 1;
            }
        }
    }
    if (swapped) {
        D_0029D9F8 = 1;
    }
end:
    func_0025EEA0();
    return 0;
}

extern void func_0023E168(void *);
int func_0020C018(void) {
    if (D_0029D9E4 != 0) {
        func_0023E168(&D_00663B40);
        D_0029D9E4 = 0;
    }
    func_0025EEA0();
    return 0;
}

extern int func_00243CE8(int);
void func_0020C050(int value) {
    do {} while (func_00243CE8(0) == value);
    D_0029D9E0 = 1;
}

void func_0020C090(void)
{
    D_0029D9E0 = 0;
}
