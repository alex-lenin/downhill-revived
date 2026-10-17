/* UNIT 0x00159930 0x00159CE0 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001CFA00_8_func_001CFA00 func_001CFA00
#else
#define CallScalarBridge001CFA00_8_func_001CFA00 ScalarBridge001CFA00_8
#endif
#include "object_registry.h"
#include "scene.h"
typedef struct Block { int x0,x4,x8,xC; } Block;
typedef struct Header { unsigned int flags:7; signed int id:11; unsigned int rest:14; } Header;
typedef SceneNode Model;
typedef struct Config { char pad[0x28B4]; float x28B4; char pad2[8]; float x28C0; char pad3[0x68]; int x292C; } Config;
typedef struct Pair { void *x0; int x4; short x8,xA; } Pair;
extern char D_003041B0[];
extern int func_00222BD8(int,int),func_00163D40(void *,int);

#if defined(__mips__)
extern void func_0022DDD8(void *),func_00159CE0(void *,int),func_00203590(void *,int,int,int),func_00203CB8(Block *),func_001CFA00(void *,long);
#else
extern void func_0022DDD8(void *);extern void func_00159CE0(void *,int);extern void func_00203590(void *,int,int,int);extern void func_00203CB8(Block *);
#endif


typedef struct Obj {
    char pad0[8]; Model *x8; int xC; char pad10[0x10]; Config *x20; char pad24[4];
    Block entries[11]; Pair pair; char padE4[0xC]; float anim[0x14]; float mat[0x10];
    void *x180; char pad184[0x58]; int x1DC; char pad1E0[8];
    short x1E8; char pad1EA[2]; short x1EC; short x1EE; char x1F0[0x10];
} Obj;
void func_00159930(Obj *self, Config *cfg, Model *model, int a3, int id, int handle) {
    char *sub; long special; float *v, *q; Block *e2; float f, g; Block copy; Model *m; int res;
    self->x20=cfg; self->x8=model; self->xC=a3;
    self->x1E8=func_00222BD8(1,(int)(cfg->x28C0*60.0f));
    sub=(char *)&self->pair;
    self->x1DC=0; self->x1EC=0;
    self->pair.x0=D_003041B0;
    res=handle;
    if(!res) res=func_00163D40(D_003041B0,0);
    *(int *)(sub+4)=res; *(short *)(sub+10)=-1; *(short *)(sub+8)=-1;
    v=self->anim; q=self->mat;
    v[0x12]=0.5f; v[0x13]=2.0f; q[0xC]=2.0943952f;
    f=self->x20->x28B4; f=(f+f)*60.0f; g=f+f;
    q[0xD]=g; q[0xE]=f; q[0xF]=g;
    { void *active=self->x180; if(active) { func_0022DDD8(active); self->x180=0; } }
    func_00159CE0(self,0);
    e2=&self->entries[2];
    func_00203590(&self->entries[0],1200,id,1);
    special=(long)e2;
    func_00203590(&self->entries[1],1200,id,2);
    func_00203590(e2,1200,id,self->x20->x292C);
    func_00203590(&self->entries[3],1200,id,4); func_00203590(&self->entries[4],1200,id,5);
    func_00203590(&self->entries[5],1200,id,6); func_00203590(&self->entries[6],1200,id,7);
    func_00203590(&self->entries[7],1200,id,8); func_00203590(&self->entries[8],1200,id,12);
    func_00203590(&self->entries[9],1200,id,13); func_00203590(&self->entries[10],1200,id,14);
    if(self->entries[0].x0) { copy=self->entries[0]; func_00203CB8(&copy); }
    if(self->entries[1].x0) { copy=self->entries[1]; func_00203CB8(&copy); }
    if(self->entries[2].x0) { copy=self->entries[2]; func_00203CB8(&copy); }
    if(self->entries[3].x0) { copy=self->entries[3]; func_00203CB8(&copy); }
    if(self->entries[5].x0) { copy=self->entries[5]; func_00203CB8(&copy); }
    if(self->entries[6].x0) { copy=self->entries[6]; func_00203CB8(&copy); }
    if(self->x8 && !self->x8->xB4) {
        self->x1EE=id;
        self->x8->xB4=RegisterObjectShortRetail((void *)(self), (unsigned int)(((Header *)self->x8->resource)));
        ((Header *)self->x8->resource)->id=self->x8->xB4;
    }
    CallScalarBridge001CFA00_8_func_001CFA00(self->x1F0,special);
}
