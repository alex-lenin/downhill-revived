/* UNIT 0x00202BD0 0x00202C48 */
int func_00202BD0(int values,int row) {
 int start,end,i,best; short *p;
 start=((row+1)*row)/2;
 end=((row+2)*(row+1))/2;
 best=start;
 for(i=start+1;i<end;i++) {
 p=(short *)(values+best*2);
 if(*(short *)(i*2+values)<*p) best=i;
 }
 return best-start;
}
