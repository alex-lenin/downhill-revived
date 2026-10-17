/* UNIT 0x0021E9A8 0x0021EA60 -O0 -fno-opt-stack */
extern int D_0029E1C8;
extern int D_0029E1CC;
extern int D_0029E1E4;
extern unsigned int D_00528900[];
extern int D_0052890C;

/* VIF1 DMA channel (D1) registers: the frame's render chain goes to VU1 */
#define D1_CHCR (*(volatile unsigned int *)0x10009000)
#define D1_QWC  (*(volatile unsigned int *)0x10009020)
#define D1_TADR (*(volatile unsigned int *)0x10009030)

void func_0021E9A8(void) {
    if (D_0029E1C8 || D_0029E1CC || D_0029E1E4) {
        D1_QWC = 0;
        D1_TADR = D_00528900[D_0052890C] & 0x0FFFFFFF;
        D1_CHCR = 0x145;
        D_0029E1C8 = 0;
        D_0029E1CC = 0;
        D_0029E1E4 = 0;
    }
}
