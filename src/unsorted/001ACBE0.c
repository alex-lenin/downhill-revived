/* UNIT 0x001ACBE0 0x001ACC88 */
#include "animation.h"
extern unsigned char D_004A9160[];

void func_001ACBE0(int active,int id) {
 int i,j; unsigned char *p,*q,*base;
 if(id==0xFFFF && active) {
 i=31; base=D_004A9160; p=base;
 do { unsigned char *old=p; p+=0x64; func_001AC780((AnimationController *)(old),2,1); i--; } while(i!=-1);
 j=31; q=base+0xC9F;
 do { *q=j; j--; q--; } while(j>=0);
 base[0xCA0]=0;
 }
}
