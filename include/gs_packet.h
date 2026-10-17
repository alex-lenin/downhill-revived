#ifndef DHD_GS_PACKET_H
#define DHD_GS_PACKET_H

/* EE register widths also hold on the i386 target. */
typedef unsigned long GsWord64 __attribute__((mode(DI)));
typedef long GsSigned64 __attribute__((mode(DI)));
typedef unsigned int GsQuad __attribute__((mode(TI)));
/* Builders also accept cursors inside word streams; no alignment is imposed. */
typedef struct GsPacket { unsigned int x0, x4, x8, xC; } GsPacket;

typedef union {
    GsWord64 raw;
    struct {
        GsWord64 en1:1, en2:1, crtmd:3, mmod:1, amod:1, slbg:1;
        GsWord64 alp:8, rest:48;
    } b;
} GsPmode;

typedef union {
    GsWord64 raw;
    struct {
        GsWord64 fbp:9, fbw:6, psm:5, pad0:12;
        GsWord64 dbx:11, dby:11, pad1:10;
    } b;
} GsDispFb;

typedef union {
    GsWord64 raw;
    struct {
        GsWord64 fbp:9, fbw:6, psm:6, pad0:11, fbmsk:32;
    } b;
} GsFrame;

typedef union {
    GsWord64 raw;
    struct {
        GsWord64 dx:12, dy:11, magh:4, magv:2, pad0:3;
        GsWord64 dw:12, dh:11, pad1:9;
    } b;
} GsDisplay;

typedef union {
    GsQuad wide;
    struct {
        GsWord64 nloop:15, eop:1, reserved:30, pre:1, prim:11, format:2, nreg:4;
        GsWord64 regs0:4, pad1:60;
    } b;
} GifTag;

/* 23 quadwords including the GIF tag, matching the +170 packet stride. */
typedef struct GsDisplayPacket { GifTag tag; GsWord64 data[44]; } GsDisplayPacket;
typedef struct GsDisplayContext { char pad0[0x80]; GsDisplayPacket pkt[2]; } GsDisplayContext;
typedef char GsPacket_size_check[sizeof(GsPacket) == 16 ? 1 : -1];
typedef char GifTag_size_check[sizeof(GifTag) == 16 ? 1 : -1];
typedef char GsDisplayPacket_size_check[sizeof(GsDisplayPacket) == 0x170 ? 1 : -1];
typedef char GsRegister_size_check[sizeof(GsPmode) == 8 && sizeof(GsDispFb) == 8 && sizeof(GsFrame) == 8 && sizeof(GsDisplay) == 8 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
GsPacket *func_00224660(GsPacket *cursor, unsigned int regsHigh, unsigned int regsLow, unsigned int nreg, unsigned int format, unsigned int prim, unsigned int pre, unsigned int eop, unsigned int nloop);
unsigned int *func_002246A0(unsigned int *cursor, unsigned int code, int count);
/* microAddress is a byte address in VU instruction memory, divided by eight for MSCAL. */
void func_002249E0(GsPacket *cursor, unsigned int flags, unsigned int microAddress, GsPacket *chainStart, int count);
#ifdef __cplusplus
}
#endif
#endif
