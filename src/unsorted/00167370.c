/* UNIT 0x00167370 0x00167558 */
#include "menu_items.h"
typedef struct {
char pad0[1048];
int x418;
char pad41c[68];
int x460;
char pad464[4];
int x468;
int x46c;
int x470;
char pad474[52];
int x4a8;
char pad4ac[24];
int x4c4;
char pad4c8[60];
int x504;
int x508;
int x50c;
int x510;
float x514;
float x518;
float x51c;
char pad520[32];
int x540;
int x544;
int x548;
int x54c;
float x550;
float x554;
float x558;
} Obj;
extern int D_0029DE68;
extern char D_00396C80[],D_003F52D0[];
extern void func_0018E638(Obj *,int,void *),func_0018E6B8(Obj *,int,void *);
extern void func_00191078(void *,void *,int,int,int,int);
extern void func_0020EC78(int *,int *,int *,int *,int *,int *,int *);
void func_00167370(Obj *obj) {
int a,b,c,d;
obj->x460=74; obj->x468=10;
func_0018E638(obj,108,&D_0029DE68);
obj->x418=0;
func_0018E3A8((struct SharedMenu *)(void *)(obj),109,&D_0029DE68,1);
func_0018E3A8((struct SharedMenu *)(void *)(obj),110,&D_0029DE68,1);
func_0018E3A8((struct SharedMenu *)(void *)(obj),111,&D_0029DE68,1);
func_0018E3A8((struct SharedMenu *)(void *)(obj),112,&D_0029DE68,1);
func_0018E3A8((struct SharedMenu *)(void *)(obj),113,&D_0029DE68,1);
func_0018E3A8((struct SharedMenu *)(void *)(obj),114,&D_0029DE68,1);
func_0018E6B8(obj,170,&D_0029DE68);
obj->x4a8=5; obj->x46c=1; obj->x470=20; obj->x4c4=20;
func_00191078(D_003F52D0,D_00396C80,190,142,144,2);
func_0020EC78(&obj->x508,&a,&obj->x50c,&obj->x510,&b,&c,&d);
obj->x514=(float)b; obj->x518=(float)c; obj->x51c=(float)d;
func_0020EC78(&obj->x540,&obj->x544,&obj->x548,&obj->x54c,&b,&c,&d);
obj->x550=obj->x514; obj->x554=obj->x518; obj->x558=obj->x51c;
obj->x504=0;
}
