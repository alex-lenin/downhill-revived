/* UNIT 0x0021F400 0x0021F620 .rodata=0x298B50:0x298C60 */
static const char D_00298B50[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\SIO2MAN.IRX;1";
static const char D_00298B70[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\MTAPMAN.IRX;1";
static const char D_00298B90[24] __attribute__((aligned(8))) = "cdrom0:\\MOD\\MCMAN.IRX;1";
static const char D_00298BA8[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\MCSERV.IRX;1";
static const char D_00298BC8[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\PADMAN.IRX;1";
static const char D_00298BE8[24] __attribute__((aligned(8))) = "cdrom0:\\MOD\\LIBSD.IRX;1";
static const char D_00298C00[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\CDVDSTM.IRX;1";
static const char D_00298C20[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\989SND.IRX;1";
static const char D_00298C40[32] __attribute__((aligned(8))) = "cdrom0:\\MOD\\989ERR.IRX;1";
extern void func_0025EB88(const char *,int,int);
extern void func_00245AE8(void),func_00245E78(void),func_0021F620(void),func_0021F788(void);
extern void func_00245D28(int),func_00244D88(int);
extern int func_00244F68(int,int,void *);
extern int D_00660CA0[4][2],D_00660CC0[4][2],D_00660C80[2][4];
extern char D_00660380[4][2][256];
extern unsigned char D_0029E298[4][2],D_0029E2A0[4][2];
void func_0021F400(void) {
 int port=0;
 func_0025EB88(D_00298B50,0,0);
 func_0025EB88(D_00298B70,0,0);
 func_0025EB88(D_00298B90,0,0);
 func_0025EB88(D_00298BA8,0,0);
 func_0025EB88(D_00298BC8,0,0);
 func_0025EB88(D_00298BE8,0,0);
 func_0025EB88(D_00298C00,0,0);
 func_0025EB88(D_00298C20,0,0);
 func_0025EB88(D_00298C40,0,0);
 func_00245AE8();
 func_00245D28(0);func_00245D28(1);func_00245D28(2);func_00245D28(3);
 func_00244D88(0);func_00245E78();func_0021F620();
 for(;port<2;port++) {
 int slot;
 for(slot=0;slot<4;slot++) {
 int index=port+slot*2;
 D_00660C80[port][slot]=index;
 D_00660CA0[slot][port]=port;
 D_00660CC0[slot][port]=slot;
 D_0029E298[slot][port]=func_00244F68(port,slot,D_00660380[slot][port])!=0;
 D_0029E2A0[slot][port]=0;
 }
 }
 func_0021F788();
}
