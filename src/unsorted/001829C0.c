/* UNIT 0x001829C0 0x00182B18 .sdata=0x29CCF8:0x29CD00 */
extern char D_004D30B0[],D_004D1A18[],D_0029CD00[],D_0029CD08[];
char D_0029CCF8[8]="LOADBAR";
extern void func_0020D250(void *, int);
extern void func_0020D290(void *, char *, int, char *, int, int);
extern int func_0020D170(void *, int);
extern void func_00218E88(int, int);
extern void func_00218D98(int);
void func_001829C0(void *unused, int arg1, int arg2) {
func_0020D250(D_004D30B0,arg2);
func_0020D290(D_004D30B0,D_0029CCF8,arg1,D_0029CD00,0,0);
func_00218E88(0x88C40,func_0020D170(D_004D30B0,0));
func_0020D250(D_004D1A18,arg2);
func_0020D290(D_004D1A18,D_0029CCF8,arg1,D_0029CD08,0,0);
func_00218D98(0x88C40);
}

extern char D_0029CD10[],D_004D0380[];
extern int D_0029DD68;
extern void func_0020D250(void *, int);
extern void func_0020D290(void *, char *, int, char *, int, int);
extern void func_0020B088(int, char *, int);
extern int func_0020D170(void *, int);
extern int func_00158680(void *, int, int);
void func_00182A88(int unused, int arg) {
 int value;
 func_0020D250(D_004D0380,0x100000);
 func_0020D290(D_004D0380,D_0029CCF8,arg,D_0029CD10,0,0);
 func_0020B088(0x88C40,D_0029CCF8,0);
 value=func_0020D170(D_004D0380,0);
 func_00158680(&D_0029DD68,value,1);
}
