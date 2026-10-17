/* UNIT 0x0016EFF8 0x0016F158 */
#include "object_registry.h"
#include "scene.h"
typedef struct { unsigned int x0:7; unsigned int x7:11; unsigned int x12:14; } Data;
typedef SceneNode Resource;
typedef struct { char pad[0x4478]; float x4478,x447c; } Config;
typedef struct { int x0,x4,x8; char pad[0x14]; } Entry;
typedef struct { char pad0[0x10]; char x10[0x70]; char x80[0xe0]; Entry x160[4]; char pad1e0[0x10]; Resource *x1f0; int x1f4; char pad1f8[4]; Config *x1fc; void *x200; char pad204[4]; char x208[8]; int x210; char pad214[4]; float x218,x21c; int x220; char pad224[0xc]; int x230; char x234,x235; } Obj;
extern char D_003BD580[];

extern void func_001DCDC8(void *);
extern void func_001DDB58(void *,void *);
extern void func_0016F198(Obj *,int);
extern void func_0016FC00(Obj *,int);
extern void func_0016F410(Obj *,int);
extern void func_001D3D10(void *);
void func_0016EFF8(Obj *obj,Config *config,void *arg2,Resource *resource,int arg4,int arg5,int arg6) {
    Entry *p;
    int count;
    unsigned int *data;
    void *a,*b,*c;
    obj->x1fc=config; obj->x200=arg2; obj->x1f4=arg4; obj->x1f0=resource;
    if(resource) {
        resource->flags|=((SceneNodeFlags)0x8000<<42);
        if(obj->x1f0->xB4==0) {
            obj->x230=arg5;
            obj->x1f0->xB4=RegisterObjectShortRetail((void *)(obj), (unsigned int)(obj->x1f0->resourceWords));
            data=obj->x1f0->resourceWords;
            ((Data *)data)->x7=obj->x1f0->xB4;
        }
        if((*obj->x1f0->resourceWords>>18)==0x50b) obj->x220=0;
    }
    a=obj->x10; b=obj->x80; c=obj->x208;
    p=obj->x160; count=3;
    do { p->x0=0; count--; p->x4=0; p->x8=0; p++; } while(count>=0);
    obj->x210=0; obj->x218=obj->x1fc->x4478; obj->x21c=obj->x1fc->x447c;
    func_001DCDC8(a); func_001DDB58(b,D_003BD580);
    func_0016F198(obj,arg6); func_0016FC00(obj,1); func_0016F410(obj,0); obj->x235=0; func_001D3D10(c);
}
