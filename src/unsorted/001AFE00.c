/* UNIT 0x001AFE00 0x001AFED8 */
#include "animation.h"
typedef AnimationHandle Desc;
struct Parent { char pad[0x3FA4]; int x3FA4; char pad3FA8[0x39C4]; int x796C; };
struct Obj { char pad[9]; char x9; char padA[0x5A]; Desc x64; float x74; char pad78[0x64]; Desc xDC; float xEC; struct Parent *xF0; };

extern void func_001AC7B0(void *,void *);
extern void func_001AC810(void *,float);

void func_001AFE00(struct Obj *obj,struct Parent *parent) {
 Desc desc; Desc *p;
 if(parent) obj->xF0=parent;
 p=&obj->x64;
 obj->x74=3.0f;
 func_002036C8(p,obj->xF0->x796C+0x14,0,1,obj->xF0->x3FA4);
 func_001AC7B0(obj,p);
 obj->x9=2;
 func_001AC810(obj,0.0f);
 obj->xEC=3.0f;
 func_002036C8(&obj->xDC,obj->xF0->x796C+0x15,0,1,obj->xF0->x3FA4);
 desc=obj->xDC;
 func_002038E0(&desc,1);
}
