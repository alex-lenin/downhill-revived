/* UNIT 0x0013E8F0 0x0013E9C0 */
#include "menu_items.h"
extern int D_0029DD30;
extern char D_0029C4C8[];
extern void func_0018E638(void *, int, void *);
extern void func_0018E408(void *, char *, int);
extern char *func_001FA370(int, int);
extern int func_00254D68(char *, const char *, ...);
void func_0013E8F0(char *obj, int value) {
 char *text=obj+0x556;
 *(int *)(obj+0x418)=0;
 func_0018E638(obj,0x66,&D_0029DD30);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0x67,&D_0029DD30,0);
 func_00254D68(text,D_0029C4C8,func_001FA370(*(int *)(obj+0x5ac),value));
 func_0018E408(obj,text,0);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),0,&D_0029DD30,0);
 func_0018E3A8((struct SharedMenu *)(void *)(obj),16,&D_0029DD30,1);
}
