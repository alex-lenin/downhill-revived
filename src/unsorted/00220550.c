/* UNIT 0x00220550 0x002205A0 */
extern int D_0029E290;
extern int D_00660D40[], D_00660D60[], D_00660D80[];
void func_00220550(int flag) {
 int i;
 D_0029E290=flag;
 if (!flag) {
  for(i=0;i<8;i++) {
   D_00660D60[i]=0;
   D_00660D40[i]=300;
   D_00660D80[i]=0;
  }
 }
}
