/* UNIT 0x0016C9E8 0x0016CA70 */
#include "scalar_api.h"
typedef struct Scalar0016C9E8Obj { char pad[0x44C0]; int x44C0,x44C4; } Obj;
extern char D_003BD580[];
extern float D_0029C8A8[];
extern int func_00163C58(void *,int,int,float);
extern int func_00163D40(void *,int);
int func_0016C9E8(Obj *obj,int arg) {
 int index; int result=0;
 if(obj->x44C4!=arg) { obj->x44C4=arg; obj->x44C0=-1; }
 index=func_00163C58(D_003BD580,obj->x44C0,arg,D_0029C8A8[0]);
 obj->x44C0=index;
 if(index>=0) result=func_00163D40(D_003BD580,index);
 return result;
}
