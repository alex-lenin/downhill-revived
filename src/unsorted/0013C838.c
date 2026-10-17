/* UNIT 0x0013C838 0x0013C910 */
extern int D_00362468[];
extern int func_00238F48(void);
extern void func_0018D850(void *,int,int,int);
void func_0013C838(void *obj,int active) {
 float x; int n;
 if(active) {
 x=(func_00238F48()%40)*0.025f;
 n=(int)((x-x*x)*4.0f*20.0f);
 D_00362468[0]=n+0x15a; D_00362468[1]=n+0xc0;
 func_0018D850(D_00362468,0x99,0x77,0x2e);
 } else {
 D_00362468[0]=0x15a; D_00362468[1]=0xc0;
 func_0018D850(D_00362468,0x50,0x50,0x50);
 }
}
