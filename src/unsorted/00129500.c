/* UNIT 0x00129500 0x00129820 */
#include "object_registry.h"
#include "scene.h"
typedef struct Settings { char pad[0x48]; float x48,x4C; } Settings;
typedef struct Rates { char pad[0x30]; float x30,x34,x38,x3C; } Rates;
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Ref { int x0,x4,x8,xC; } __attribute__((packed)) Ref;
typedef struct Header { unsigned int flags:7; signed int id:11; unsigned int rest:14; } Header;
typedef SceneNode Item;
typedef struct Config { char pad0[0x31B8]; float x31B8; } Config;
typedef struct Obj { int x0; Item *x4,*x8; char padC[0x14]; Config *x20; int x24,x28; Ref x2C,x3C,x4C,x5C; void *x6C; int x70; short x74,x76; char pad78[8]; Settings x80; Rates xD0; int x110; char pad114[0x2C]; char x140[0x2C]; float x16C; char pad170[0x20]; int x190,x194; } Obj;
extern void func_00129860(Obj *,int);
extern int func_00163D40(void *,int);

extern void func_001CFA00(void *,Ref *);
extern void func_00203590(Ref *,int,int,int);
extern void func_00203CB8(Ref *);
extern void func_0022DDD8(int);
extern float D_0029C24C[];
extern char D_003041B0[];
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00129500(Obj *self,Config *cfg,Item *a,Item *b,int id,int handle) {
    Vec scale; Ref tmp; char *sub; Settings *sub80; Rates *subD0; float rate, twice, initial; float *value; int duration; int active;
    scale.x=1.0f; scale.y=1.0f; scale.z=1.0f;
    self->x4=a; self->x8=b; self->x20=cfg;
    self->x6C=D_003041B0;
    sub=(char *)&self->x6C;
    if (!handle) handle=func_00163D40(D_003041B0,0);
    *(int *)(sub+4)=handle; *(short *)(sub+10)=-1; *(short *)(sub+8)=-1;
    sub80=&self->x80;
    sub80->x48=0.5f; sub80->x4C=2.0f;
    subD0=&self->xD0;
    subD0->x30=2.09439516f;
    initial=D_0029C24C[0]; rate=self->x20->x31B8; rate=(rate+rate)*60.0f; twice=rate+rate;
    subD0->x34=twice; subD0->x38=rate; subD0->x3C=twice;
    value=D_0029C24C; duration=300; self->x16C=initial; self->x194=duration; self->x24=0; active=self->x110;
    if(active) { func_0022DDD8(active); self->x110=0; }
    func_00129860(self,0);
    if(self->x4) {
        self->x4->flags|=(SceneNodeFlags)0x8000<<42;
        self->x4->flags|=(SceneNodeFlags)0x8000<<41;
        if(!self->x4->xB4) {
            self->x190=id;
            self->x4->xB4=RegisterObjectShortRetail((void *)(self), (unsigned int)(((Header *)self->x4->resource)));
            ((Header *)self->x4->resource)->id=self->x4->xB4;
        }
        func_00203590(&self->x3C,0x4B5,id,2);
        func_00203590(&self->x2C,0x4B5,id,1);
        func_00203590(&self->x4C,0x4B5,id,3);
        func_00203590(&self->x5C,0x4B5,id,4);
        scale.x=1.5f; scale.y=1.5f; scale.z=1.5f;
        COPY_VECTOR(self->x4->x30,&scale);
    }
    if(self->x8) { self->x8->flags|=(SceneNodeFlags)0x8000<<41; COPY_VECTOR(self->x8->x30,&scale); }
    if(self->x3C.x0) { tmp=self->x3C; func_00203CB8(&tmp); }
    if(self->x2C.x0) { tmp=self->x2C; func_00203CB8(&tmp); }
    if(self->x4C.x0) { tmp=self->x4C; func_00203CB8(&tmp); }
    if(self->x5C.x0) { tmp=self->x5C; func_00203CB8(&tmp); }
    func_001CFA00(self->x140,&self->x4C);
}
