/* UNIT 0x001F8628 0x001F87B0 */
#include "timer.h"
typedef struct { int x0; char p4[0x414]; int x418; char p41c[0x84]; int x4A0,x4A4; } Obj;
typedef struct { char p0[0x10c]; int x10C; char p110[0x180]; int x290; } State;
typedef struct { int x0,x4,x8,xC; } Local;
extern State D_004B25A0;
extern char D_004225A0[],D_00759780[];
typedef struct { char pad[0x12D]; char x12D; } Flags;
extern Flags D_003F52D0;
extern int *D_0029E050;
extern int func_001F8458(int),func_001F8440(int),func_00193F68(void *,int);
extern void func_0018E408(Obj *,int,int),func_00203590(Local *,int,int,int),func_002038E0(Local *,int),func_00262728(void);
extern int func_001B5AF0(void *,int,int);
void func_001F8628(Obj *obj) {
 Local locals[2]; int i; 
 obj->x0=0;
 if(!func_001F8458(D_004B25A0.x10C)) { D_003F52D0.x12D=1; return; }
 obj->x4A0=2; i=0; obj->x4A4=3; obj->x418=0;
 while(func_001F8440(i)>=0) {
  int offset=i*4;
  int *p=(int *)(offset+(int)D_0029E050);
  void *table=D_004225A0;
  int n=func_001F8440(i);
  i++;
  func_0018E408(obj,*p,func_00193F68(table,n));
 }
 locals[0].x8=0;
 func_00203590(&locals[0],0x2588,0,D_004B25A0.x290/32+3);
 func_002038E0(&locals[0],1);
 {
 locals[1].x8=0;
 func_00203590(&locals[1],0x23d7,0,1);
 func_002038E0(&locals[1],1);
 func_001B5AF0(&D_003F52D0,2,1);
 func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00262728), obj, 10);
 }
}
