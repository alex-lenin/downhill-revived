/* UNIT 0x001B57F8 0x001B5A38 */
#include "ui_style.h"
#include "disc_path.h"
typedef struct { unsigned char bytes[9]; char pad9[0x3F]; int x48; char pad4C[0x14]; unsigned char x60; } Obj;
typedef struct { char pad0[0x20]; int x20; } Display;
typedef struct { char pad0[0x1D8]; int x1D8; } State;
typedef struct { char pad0[0x1680]; int x1680; } Manager;
extern Display D_003F52D0;
extern State *D_0029DD30;
extern Manager D_004D1A18;
extern char D_004B1F50[],D_0029D418[],D_0029D420[],D_0029D428[];
extern struct { char pad0[0xB18C]; int xB18C; } D_004B25A0;
extern void func_001B5788(Obj *);
extern void func_001B5AF0(Obj *,int,int);
extern void func_0020F2A8(int);
extern void func_0023D650(int,int *,int *);
extern void func_0021F0A0(int,int,int);
extern void func_001B4DD0(Obj *);
extern void func_0020F9A8(int,int,int,int,int);
extern void func_001B5130(Obj *);
extern void func_0020D080(Manager *,int);

extern int func_0020D170(Manager *,int);
extern int func_00241F90(void *,int);
extern void func_00218D88(int);
extern void func_00218D98(int);
extern int func_0021F820(void);
extern void func_00190688(Display *);
void func_001B57F8(Obj *o) {
 unsigned char saved[9]; char work[64]; int width,height; int i=0;
 func_001B5788(o); func_001B5AF0(o,4,0); func_001B5AF0(o,3,0);
 do { saved[i]=o->bytes[i]; i++; } while(i<9);
 o->bytes[1]=0; o->bytes[5]=1; o->bytes[8]=1;
 i=14; func_0020F2A8(0); func_0023D650(D_003F52D0.x20,&width,&height);
 width=(int)((float)width*0.9f); height=(int)((float)height*0.9f);
 func_0021F0A0(o->x48,0,0); func_0021F0A0(o->x48,0,1); o->x60=1;
 do { i--; func_001B4DD0(o); func_0018D800((const UiStyle *)(void *)(D_004B1F50)); func_0020F9A8(0,D_003F52D0.x20,20,21,D_0029DD30->x1D8); func_001B5130(o); } while(i>=0);
 i=0; do { o->bytes[i]=saved[i]; i++; } while(i<9);
 func_0020D080(&D_004D1A18,0);
 BuildDiscPathWithMode((char *)(work), (const char *)(D_0029D418), (const char *)(D_0029D420), (const char *)(D_0029D428), 0);
 func_00241F90(work,func_0020D170(&D_004D1A18,0));
 D_004D1A18.x1680=1; func_00218D88(D_004B25A0.xB18C); func_00218D98(0x69840); func_0021F820(); func_00190688(&D_003F52D0);
}
