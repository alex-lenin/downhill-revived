/* UNIT 0x00182330 0x001824A8 */
struct Global { char pad0[0x10C]; int x10C,x110; char pad114[0x18C]; int x2A0; };
extern struct Global D_004B25A0;
extern char D_0029CCC0[],D_0029CCC8[],D_0029CCD0[],D_0029CCD8[],D_0029CCE0[],D_0029CCE8[],D_0029CCF0[],D_0028C528[],D_003C6350[];
extern char *D_0026EF60[],*D_0026F0D8[];
extern int func_00254D68(char *,char *,...);
char *func_00182330(int a,int b) {
 if(D_004B25A0.x10C==20) return D_0029CCC0;
 if(D_004B25A0.x2A0==21) return D_0029CCC8;
 if(b==17) return D_0029CCD0;
 if(b==15) return D_0029CCD8;
 if(b==11) return D_0029CCE0;
 if((unsigned)(b-6)<4) goto single;
 if(D_004B25A0.x110==1) {
  func_00254D68(D_003C6350,D_0029CCF0,D_0026EF60[a],D_0026F0D8[b]);
  return D_003C6350;
 }
 if(b==12 || b==14 || b==18 || b==10 || b==19 || b==13) {
single:
  func_00254D68(D_003C6350,D_0029CCE8,D_0026EF60[a]);
  return D_003C6350;
 }
 return D_0028C528;
}
