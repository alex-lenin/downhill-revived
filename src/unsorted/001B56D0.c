/* UNIT 0x001B56D0 0x001B5788 */
/* Starts DMA channel 2 (GIF) on the packet D_004B2030. */
typedef struct Obj {
    char pad0[0x44];
    int x44;
} Obj;

extern struct Packet {
    char pad0[0x20];
    unsigned char x20;
} D_004B2030;
extern int D_0029E110;
extern void func_0025A960(int);
extern int func_002433A8(int, int);

void func_001B56D0(Obj *o) {
    D_004B2030.x20 = o->x44;
    func_0025A960(0);
    func_002433A8(0, 0);
    *(volatile unsigned int *)0x1000a020 = 0;                                       /* D2_QWC */
    *(volatile unsigned int *)0x1000a030 = (unsigned int)&D_004B2030 & 0x7fffffff; /* D2_TADR */
    *(volatile unsigned int *)0x1000e010 = 4;                                       /* D_STAT */
    *(volatile unsigned int *)0x1000a000 = 0x145;                                   /* D2_CHCR */
    func_002433A8(0, 0);
    o->x44 = D_0029E110 == 0 ? 3 : 2;
}
