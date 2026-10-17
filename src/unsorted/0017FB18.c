/* UNIT 0x0017FB18 0x0017FDB8 */
#include "object_registry.h"
#include "scene.h"
#include "animation.h"
typedef SceneNode Entity;
typedef struct { char pad0[0x5670]; float x5670,x5674; } Params;
typedef struct { int x0,x4,x8; char padC[0x14]; } Record;
typedef struct { void *x0; int x4; short x8,xA; char padC[0xC]; } Handle;
typedef struct { char pad0[0x48]; float x48; int x4C; } SubF0;
typedef struct { char pad0[0x30]; float x30,x34,x38,x3C; } Sub140;
typedef struct { char pad0[8]; Entity *x8; int xC; char pad10[0x10]; Record x20[5]; char padC0[0x10]; Params *xD0; int xD4; Handle xD8; SubF0 xF0; Sub140 x140; char pad180[0x20]; float x1A0,x1A4; char pad1A8[0xC]; int x1B4; char pad1B8[8]; char x1C0[0x50]; int x210; } Obj;
typedef AnimationHandle Temp;
extern char D_003BD580[];

extern int func_00163D40(void *,int);
extern void func_00180E78(Obj *,int),func_0017FFB8(Obj *,int);
extern void func_00211280(Entity *);
extern float func_00222B68(float,float);
extern void func_001D3370(void *,Obj *);
void func_0017FB18(Obj *obj, Params *params, Entity *entity, int arg3, int arg4, int arg5) {
    Temp temp;
    Record *record;
    Handle *handle;
    SubF0 *subF0;
    Sub140 *sub140;
    void *sub1C0;
    int count, handleId;
    unsigned int flags;
    float value;
    obj->xC=arg3;
    obj->x8=entity;
    if (entity) {
        entity->flags |= (SceneNodeFlags)0x8000 << 42;
        if (!obj->x8->xB4) {
            obj->x1B4=arg4;
            obj->x8->xB4=RegisterObjectShortRetail((void *)(obj), (unsigned int)(obj->x8->resourceWords));
            *obj->x8->resourceWords=(*obj->x8->resourceWords & 0xFFFC007F) | (((obj->x8->xB4 % 2048) & 0x7FF)<<7);
        }
    }
    obj->xD0=params;
    subF0=&obj->xF0;
    handle=&obj->xD8;
    sub140=&obj->x140;
    sub1C0=obj->x1C0;
    record=obj->x20;
    count=5;
    do { record->x0=0; count--; record->x4=0; record->x8=0; record++; } while (count>=0);
    handleId=arg5;
    obj->xD8.x0=D_003BD580;
    if (!handleId) handleId=func_00163D40(D_003BD580,0);
    handle->x4=handleId;
    handle->xA=-1;
    handle->x8=-1;
    subF0->x48=0.5f;
    subF0->x4C=0;
    if (obj->x8) {
        flags=*obj->x8->resourceWords & 0xFFFC0000;
        if (flags==0x15D00000 || flags==0x16000000) subF0->x48=0.25f;
    }
    sub140->x30=2.0943952f;
    func_00180E78(obj,1);
    func_00211280(obj->x8);
    obj->x1A0=obj->xD0->x5670;
    value=func_00222B68(obj->xD0->x5670,obj->xD0->x5674);
    obj->x1A4=value;
    value*=60.0f;
    sub140->x34=value;
    sub140->x3C=value;
    sub140->x38=value;
    func_0017FFB8(obj,0);
    func_001D3370(sub1C0,obj);
    obj->x210=0;
    if ((*obj->x8->resourceWords & 0xFFFC0000)==0x14500000) {
        temp.x8=0;
        func_00203590(&temp,0x514,arg4,0);
        if (temp.x0) func_00203E10(&temp,2.2f);
    }
    if ((*obj->x8->resourceWords & 0xFFFC0000)==0x14580000) {
        temp.x8=0;
        func_00203590(&temp,0x516,arg4,0);
        if (temp.x0) func_00203E10(&temp,2.5f);
    }
}
