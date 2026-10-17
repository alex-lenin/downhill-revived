/* UNIT 0x001AAD00 0x001AAE68 */
#include "sound_fades.h"
#if defined(__mips__)
#define CallShared001CF5C8_0 func_001CF5C8
#else
#define CallShared001CF5C8_0 Forward001CF5C8_1
#endif
#include "timer.h"
typedef struct Limit { char pad[12]; float xC; } Limit;
typedef struct State { char pad[0x18]; float x18; char pad1C[8]; int x24; Limit *x28; char pad2C[0xD4]; float x100[4]; } State;
typedef struct Body { long flags; char pad8[8]; float pos[4]; } Body;
typedef struct Obj { char pad[0x20]; State *x20; Body *x24; short x28; char pad2A[2]; char x2C[1]; } Obj;
extern char D_002AF2C0[], D_00759780[];
extern void func_001AAF40(Obj*), func_001AB418(Obj*), func_001AA688(Obj*),func_002624E0(void);
extern void func_0011C208(void*,void*,void*,void*,float*);
extern void func_001CF5C8(void*,void*,int,float);
static inline float magnitude(float *v) { float r; __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(r) : "f"(v[0]),"f"(v[1]),"f"(v[2])); return r; }
int func_001AAD00(Obj *obj) {
 void *a,*b; float distance, root; float *v; long mask;
 func_001AAF40(obj); mask=1L<<57; func_001AB418(obj);
 if (!(obj->x24->flags & mask)) {
 func_0011C208(D_002AF2C0,obj->x24->pos,&a,&b,&distance);
 __asm__ volatile ("sqrt.s %0,%1" : "=&f"(root) : "f"(distance));
 if(obj->x20->x28->xC+4.0f < root) obj->x24->flags |= mask;
 }
 v=obj->x20->x100;
 if(magnitude(v)<obj->x20->x18) {
 obj->x20->x24++;
 if(obj->x20->x24>=21) { func_001AA688(obj); func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_002624E0), obj, 1); return 0; }
 } else { obj->x20->x24=0; CallShared001CF5C8_0(obj->x2C,obj->x24->pos,obj->x28,magnitude(v)); }
 return 1;
}
