/* UNIT 0x0019EA38 0x0019ECB8 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001554B8_6_func_001554B8 func_001554B8
#else
#define CallScalarBridge001554B8_6_func_001554B8 ScalarBridge001554B8_6
#endif
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj { char pad0[8]; int x8,xC; char pad10[4]; int x14; char pad18[0x2C]; int x44; char pad48[4]; int x4C; char pad50[8]; unsigned char x58[10]; char pad62[0x11E]; Vec x180,x190; char pad1A0[0x8C]; unsigned char x22C; char pad22D[0x25B]; int x488; } Obj;
typedef struct Rider { char pad0[0x3FD0]; char x3FD0[0xB8]; int x4088; char pad408C[0x39AC]; int x7A38; char pad7A3C[0x194]; } Rider;
extern Rider D_002AF2C0[];
typedef struct World { char pad0[0x5B74]; char x5B74[4]; } World;
extern World D_00398C40;
typedef struct Manager { char pad0[0x150]; char x150[4]; } Manager;
extern Manager D_003B2270;
extern char D_004348F0[],D_0043F870[],D_004BD740[];
extern int D_003B01CC[],D_0036CFB4[],D_00386668[],D_0032FC4C[],D_003BD544[];
extern void func_0019ECB8(Obj *,int,int);
extern void func_001C12B8(void *),func_001C1300(void *),func_0011ADE8(void *),func_0017BE88(void *),func_00196460(void *),func_001C2880(void *);
extern int func_001C2A90(void *);

#if defined(__mips__)
extern void func_001554B8(void *,float),func_00238A68(int),func_00203590(void *,int,int,int),func_002038E0(Vec *,int),func_00203B68(Vec *,int,float),func_00203F40(Vec *,int),func_00176E40(void *,int);
#else
extern void func_00238A68(int);extern void func_00203590(void *,int,int,int);extern void func_002038E0(Vec *,int);extern void func_00203B68(Vec *,int,float);extern void func_00203F40(Vec *,int);extern void func_00176E40(void *,int);
#endif

extern int func_00238F48(void);
void func_0019EA38(Obj *obj,int reset,int mode) {
 Vec v; int i,j; int *p; Rider *r; Manager *manager; World *world;
 if(reset) { if(mode) func_0019ECB8(obj,reset,mode); else { void *q; world=&D_00398C40; q=world->x5B74; func_001C12B8(q); func_001C1300(q); obj->x488=1; } }
 obj->x8=0; obj->xC=0;
 if(!obj->x22C) func_0011ADE8(D_002AF2C0);
 if(mode) obj->x22C=1;
 func_0017BE88(&D_003B2270); func_00196460(D_004348F0);
 D_0036CFB4[0]=1; D_00386668[0]=1; D_0032FC4C[0]=1; D_003BD544[0]=1; D_003B01CC[0]=1;
 for(i=0;i<10;i++) { r=&D_002AF2C0[i]; obj->x58[i]=0; p=&r->x7A38; j=0x22; do { *p=0; --j; --p; } while(j>=0); { void *q=r->x3FD0; *(int *)((char *)q+0xB8)=0; CallScalarBridge001554B8_6_func_001554B8(q,1.0f); } }
 obj->x44=0; obj->x4C=0; func_00238A68(60); obj->x14=func_00238F48(); { char *q=D_0043F870; *(int *)(q+0x140)=1; }
 func_00203590(&obj->x190,0x2198,0,0);
 if(mode) {v=obj->x190; func_002038E0(&v,1);} else {v=obj->x190; func_00203B68(&v,1,1.0f);}
 v=obj->x180; func_00203F40(&v,1); func_001C2880(D_004BD740); if(func_001C2A90(D_004BD740)) { manager=&D_003B2270; func_00176E40(manager->x150,0); }
}
