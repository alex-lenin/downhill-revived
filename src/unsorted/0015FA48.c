/* UNIT 0x0015FA48 0x0015FAD0 */
#include "menu_items.h"
extern int D_0029DE68;
extern char D_003F52D0[];
extern void func_0018E638(void *,int,void *);
extern void func_001B5AF0(void *,int,int);
void func_0015FA48(char *obj) {
 func_0018E638(obj,0x21,&D_0029DE68);
 func_001B5AF0(&D_003F52D0,1,1);
 *(int *)(obj+0x418)=0;
 *(int *)(obj+0x45C)=16;
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x1A,&D_0029DE68,1);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x22,&D_0029DE68,1);
 *(int *)(obj+0x460)=80;
}
