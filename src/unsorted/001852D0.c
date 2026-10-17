/* UNIT 0x001852D0 0x00185370 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00185730_19_func_00185730 func_00185730
#else
#define CallScalarBridge00185730_19_func_00185730 ScalarBridge00185730_19
#endif
typedef struct Obj { char p0[0xc88]; int xC88; char pC8C[8]; unsigned char xC94; } Obj;
typedef struct Input { int x0; char p4[0x9d]; char xA1[1]; } Input;

#if defined(__mips__)
extern void func_00185730(long, long);
#else

#endif

extern int func_00185938(Obj *, char *);
extern void func_00185488(Obj *, Input *, unsigned char *);
extern char D_0028C648[];
int func_001852D0(Obj *o, Input *in) {
 unsigned char val;
 if(in->x0==0) {
 o->xC94=0;
 CallScalarBridge00185730_19_func_00185730((long)o,(long)in);
 if(o->xC88>=5) {
 if(func_00185938(o,D_0028C648) && func_00185938(o,in->xA1)) {
 func_00185488(o,in,&val); o->xC94=val; return 1;
 }
 }
 }
 return 1;
}
