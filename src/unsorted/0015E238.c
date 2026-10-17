/* UNIT 0x0015E238 0x0015E448 */
#include "menu_items.h"
typedef struct { int x0; char pad4[0x414]; int x418; char pad41C[0x44]; int x460,x464,x468,x46C,x470; } Menu;
typedef struct { char pad0[0x130]; int x130; } Manager;
typedef struct { char pad0[0x10C]; int x10C,x110; char pad114[0x174]; int x288; } State;
typedef struct { char pad0[0x12199]; unsigned char x12199; char pad1219A[5]; unsigned char x1219F; } Save;
extern Manager D_003F52D0;
extern State D_004B25A0;
extern Save D_004225A0;
extern void *D_0029DE68;
extern void func_001B5AF0(Manager *,int,int),func_0018E638(Menu *,int,void **);
void func_0015E238(Menu *menu) {
 State *state;
 menu->x0=0; menu->x460=0x54; menu->x468=10;
 func_001B5AF0(&D_003F52D0,1,1);
 func_0018E638(menu,D_003F52D0.x130+20,&D_0029DE68);
 menu->x418=0;
 func_0018E3A8((struct SharedMenu *)(void *)(menu),0x19,&D_0029DE68,1);
 if(D_004B25A0.x10C==5 && D_004B25A0.x288==1) func_0018E3A8((struct SharedMenu *)(void *)(menu),0x7d,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(menu),0x48,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(menu),0x47,&D_0029DE68,1);
 if(D_004B25A0.x110==0 && D_004B25A0.x10C!=15 && D_004B25A0.x10C!=16 && D_004B25A0.x10C!=17 && D_004B25A0.x10C!=11) func_0018E3A8((struct SharedMenu *)(void *)(menu),0x29,&D_0029DE68,1);
 state=&D_004B25A0;
 if(state->x10C!=17) func_0018E3A8((struct SharedMenu *)(void *)(menu),0x22,&D_0029DE68,1);
 if(D_004225A0.x12199) func_0018E3A8((struct SharedMenu *)(void *)(menu),0x6c,&D_0029DE68,1);
 if(D_004225A0.x1219F && state->x288==1) func_0018E3A8((struct SharedMenu *)(void *)(menu),0x3f,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(menu),0x85,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(menu),0x1a,&D_0029DE68,1);
 menu->x470=0; menu->x46C=1;
}
