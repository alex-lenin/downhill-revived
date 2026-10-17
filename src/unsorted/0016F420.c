/* UNIT 0x0016F420 0x0016F500 */
#include "sound_fades.h"
#if defined(__mips__)
#define CallShared001D3D20_0 func_001D3D20
#else
#define CallShared001D3D20_0 Forward001D3D20_0
#endif
struct Obj { char p0[0x84]; int x84; char p88[0x168]; char *x1F0; char p1F4[0x10]; int x204; char x208[8]; float x210,x214; char p218[0x14]; int x22C; };
extern int func_00238F60(void);
extern void func_0016F198(struct Obj *,int),func_0016FC00(struct Obj *,int),func_0016F410(struct Obj *,int),func_0016F6E0(struct Obj *),func_0016F710(struct Obj *),func_0016F500(struct Obj *);
extern void func_001D3D20(float,void *,float,void *);
void func_0016F420(struct Obj *obj) {
 int mode;
 obj->x22C+=func_00238F60(); mode=obj->x204;
 switch(mode) {
 case 0:
 if(!obj->x1F0) return;
 func_0016F198(obj,obj->x84); func_0016FC00(obj,1); func_0016F410(obj,1); break;
 case 1: func_0016F6E0(obj); break;
 case 2: func_0016F710(obj); break;
 }
 if(obj->x1F0) {
 CallShared001D3D20_0(obj->x210,obj->x208,obj->x214,obj->x1F0+16);
 func_0016F500(obj);
 }
}
