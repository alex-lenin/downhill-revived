/* UNIT 0x00177DA0 0x00177F08 */
#include "soft_double.h"
struct Race { char pad[0x4098]; unsigned long flags; char pad40A0[0xE0]; float velocity[4]; };
struct Obj { char pad[0xF24]; long *xF24; char padF28[0x23C]; struct Race *x1164; char pad1168[0x1DC]; unsigned char x1344,x1345,x1346,x1347,x1348; };
extern float D_0029DD88;
extern int D_0043F348[];
extern char D_0029C9D0[];

extern int func_00254D68(char *, const char *, ...);
void func_00177DA0(struct Obj *obj) {
 char buf[16]; float length,value; unsigned int digit; float *v=obj->x1164->velocity;
 __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(v[0]),"f"(v[1]),"f"(v[2]));
 value=length*0.6818182f; value*=D_0029DD88;
 if(((unsigned char)(obj->x1164->flags>>2))&1) value=0.0f;
 if(value<1.0f) value=0.0f;
 if(D_0043F348[0]==300) value=0.0f;
 if(value>99.0f) value=99.0f;
 func_00254D68(buf,D_0029C9D0,func_001FD300(value));
 obj->x1344=obj->x1347;
 digit=buf[1]-'0'; if(digit>=10) digit=0; obj->x1347=digit;
 digit=buf[0]-'0'; if(digit>=10) digit=0;
 if(digit==0) *obj->xF24=(*obj->xF24 & ~0x3FL)|9;
 else *obj->xF24 &= ~0x3FL;
 obj->x1345=obj->x1348; obj->x1348=digit;
}
