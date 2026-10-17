/* UNIT 0x0021F8E0 0x0021FAD0 */
extern int D_00660C80[2][4];
extern unsigned char D_0029E298[], D_00660E10[][6];
extern int D_00660E40[], D_00660D40[], D_00660D60[], D_00660D80[], D_0029E290;
extern void func_0021FAD0(int,int), func_00220018(int,int,int,int), func_00220130(int);
extern int func_00220180(int),func_002201B0(int);
void func_0021F8E0(void) {
 int i,j,k,id; int *p; unsigned char *q;
 i=0; do { int ni, offset; j=0; offset=i*16; ni=i+1; do {
  id=*(int *)(j*4+offset+(int)D_00660C80);
  if(D_0029E298[id]) {
   q=(unsigned char *)(id*6+(int)D_00660E10); k=1;
   func_0021FAD0(i,j);
   p=(int *)(id*8+(int)D_00660E40);
   do { int n=*p; int next=n-1; k--; if(n) {*p=next; if(next<=0) {*q=0;func_00220018(id,0,0,0);}} q++;p++; } while(k>=0);
  }
 j++; }while(j<4); i=ni; }while(i<2);
 if(!D_0029E290) for(k=0;k<8;k++) {
  if(func_002201B0(k)) {
   if(D_00660D80[k]) {if(++D_00660D60[k]>D_00660D40[k]) {D_00660D60[k]=0;D_00660D40[k]=300;}}
   else D_00660D60[k]=0;
  } else D_00660D80[k]=0;
 }
 for(k=0;k<8;k++) if(func_00220180(k)) func_00220130(k);
}
