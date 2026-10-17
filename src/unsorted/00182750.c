/* UNIT 0x00182750 0x00182818 */
extern char D_00285E98[];
extern char D_00285E70[];
extern char D_00285E48[];
extern char D_00285E20[];
extern char D_00285DF8[];
extern char D_00285DD0[];
extern char D_00285DA8[];
extern char D_00285D80[];
extern char D_00285EC0[];
extern char *D_0029DD94[];
extern char *D_0029DD9C[];
extern char *D_0029DDA4[];
extern char *D_0029DDAC[];
extern char *D_0029DDB4[];
extern char *D_0029DDBC[];
extern char *D_0029DDC4[];
extern char *D_0029DDCC[];
extern char *D_0029DDD4[];
void func_00182750(int enabled, int reason) {
if (reason==0xFFFF && enabled) {
char *last=D_00285EC0;
D_0029DD94[0]=D_00285E98;
D_0029DD9C[0]=D_00285E70;
D_0029DDA4[0]=D_00285E48;
D_0029DDAC[0]=D_00285E20;
D_0029DDB4[0]=D_00285DF8;
D_0029DDBC[0]=D_00285DD0;
D_0029DDC4[0]=D_00285DA8;
D_0029DDCC[0]=D_00285D80;
D_0029DDD4[0]=last;
}
}
