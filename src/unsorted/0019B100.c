/* UNIT 0x0019B100 0x0019B168 */
typedef struct Obj { char p[0x50C]; char x50C[4][13]; char x540[4]; } Obj;
void func_0019B100(Obj *obj) {
 int row, col;
 for(row=0;row<4;row++) {
  for(col=12;col>=0;col--) obj->x50C[row][col]=40;
  obj->x540[row]=0;
 }
}
