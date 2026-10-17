/* UNIT 0x001835E0 0x00183678 */
typedef struct {char p[0x24]; int x24,x28; char p2c[12]; unsigned *x38[50]; unsigned *x100[50]; int x1c8[50]; } Obj;
void func_001835E0(Obj *o,unsigned *p){
unsigned word=*p; unsigned code=word>>18;
if(code-0x33f5U<0x31U){
o->x38[code-0x33f4]=p;
o->x1c8[code-0x33f4]=(*p>>7)&0x7ff;
*p &= 0xfffc007f;
o->x24++; return;
}
if((word&0xfffc0000)==0xd0980000){int n=o->x28; o->x100[n]=p;o->x28=n+1;}
}
