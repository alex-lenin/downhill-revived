/* UNIT 0x001C0768 0x001C0958 */
extern int D_004B2840[];
unsigned int func_001C0768(int unused,int id) {
 unsigned int result=0xffffffff;
 switch(id) {
 case 0x4bf: case 0x4c1: case 0x4c3: result=0; break;
 case 0x4b8: result=1; break;
 case 0x4c9: result=2; break;
 case 0x4b5:
  if(D_004B2840[0]==3 || D_004B2840[0]==9 || D_004B2840[0]==4 || D_004B2840[0]==12 || D_004B2840[0]==21) result=10;
  else result=3;
  break;
 case 0x5cd: case 0x5cf:
  if(D_004B2840[0]==20) result=14; break;
 case 0x5d1: case 0x5d3:
  if(D_004B2840[0]==20) result=13; break;
 case 0x4ba: result=4; break;
 case 0x4c6:
  if(D_004B2840[0]==5 || D_004B2840[0]==22) result=6;
  else result=9;
  break;
 case 0x4d3:
  if(D_004B2840[0]==5 || D_004B2840[0]==22 || D_004B2840[0]==14 || D_004B2840[0]==0 || D_004B2840[0]==6 || D_004B2840[0]==24) break;
  result=5; break;
 case 0x4d5: result=8; break;
 }
 return result;
}
