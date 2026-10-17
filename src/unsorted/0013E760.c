/* UNIT 0x0013E760 0x0013E8F0 */
#include "menu_items.h"
typedef struct { char pad[0x184]; int x184, x188; } Data;
typedef struct { char pad[0x418]; int x418; char pad41C[0xE4]; char x500[0x56]; char x556[0x56]; int x5AC, x5B0, x5B4; } Obj;
extern Data *D_0029DD30;
extern char D_004225A0[0x12158];
extern char D_0029C4E0[], D_0029C4C8[];
extern int func_001FA300(int, int);
extern char *func_001FA370(int,int);
extern void func_001940E0(void *,int), func_00194128(void *,int), func_00194170(void *,int);
extern void func_0018E638(Obj *,int,Data **), func_0018E408(Obj *,char *,int);
extern int func_00254D68();
void func_0013E760(Obj *obj,int value) {
 char *a, *b;
 obj->x5B0=value;
 obj->x5B4=func_001FA300(obj->x5AC,value)/2;
 switch(obj->x5AC) {
 case 1: func_001940E0(D_004225A0,value); break;
 case 2: func_00194128(D_004225A0,value); break;
 case 3: func_00194170(D_004225A0,value); break;
 }
 obj->x418=0;
 func_0018E638(obj,0x60,&D_0029DD30);
 a=obj->x500; b=obj->x556;
 func_00254D68(a,D_0029C4E0,D_0029DD30->x184,obj->x5B4,D_0029DD30->x188);
 func_00254D68(b,D_0029C4C8,func_001FA370(obj->x5AC,value));
 func_0018E408(obj,a,0); func_0018E408(obj,b,0);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0,&D_0029DD30,0); func_0018E3A8((struct SharedMenu *)(void *)(obj),0x10,&D_0029DD30,1);
}
