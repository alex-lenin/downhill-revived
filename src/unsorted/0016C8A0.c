/* UNIT 0x0016C8A0 0x0016C9E8 .rodata=0x28AF30:0x28AF60 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0016C9E8_15_func_0016C9E8 func_0016C9E8
#else
#define CallScalarBridge0016C9E8_15_func_0016C9E8 ScalarBridge0016C9E8_15
#endif
typedef struct Obj { unsigned int *x0; } Obj;
typedef struct State { Obj *x0[10]; int x28[10]; char pad50[0x448c-0x50]; int x448c[10]; int x44b4,x44b8,x44bc,x44c0; } State;
extern char D_003BD580[];
extern void *func_001817E0(void *,unsigned int);

#if defined(__mips__)
extern int func_0016C9E8(State *,void *,float);
#else

#endif

extern void func_0016CE68(void *,State *,Obj *,int,int,int,int);
void func_0016C8A0(State *s) {
 int i,kind=2;
 s->x44c0=-1; s->x44b4=0;
 for(i=0;i<s->x44b8;i++) {
  if(s->x0[i]) {
   int result=CallScalarBridge0016C9E8_15_func_0016C9E8(s,func_001817E0(D_003BD580,*s->x0[i]->x0>>18),250.0f);
   if(result) {
    func_0016CE68((char *)s+0x50+s->x44b4*0x6d0,s,s->x0[i],s->x28[i],s->x448c[i],result,kind);
    switch(kind) {case 2:kind=9;break;case 9:kind=5;break;case 5:kind=12;break;case 6:kind=7;break;case 7:case 12:kind=6;break;default:kind=2;break;}
    s->x44b4++;
   }
  }
 }
}
