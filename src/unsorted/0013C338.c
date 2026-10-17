/* UNIT 0x0013C338 0x0013C400 .sbss=0x29DD24:0x29DD28 */
#include "ui_style.h"
int D_0029DD24 __attribute__((section(".sbss")))=0;
extern char D_003616E0[];
typedef struct Globals { char pad[0x12208]; int x12208; } Globals;
extern Globals D_004225A0;
extern char D_0029C4D0[];
typedef struct Obj { char pad[0x4C]; int x4C; } Obj;
extern Obj *D_0029DD30;
extern void func_00254D68(void *,void *,int,int);
extern void func_0018D930(void *,int,int,void *);
void func_0013C338(void *obj) {
 char text[32];
 if(!D_0029DD24) {
  InitializeUiStyleDiscard((UiStyle *)(void *)(D_003616E0),0x180,0xD8,-0x59,-0x21,0x2A,0xA2,0x2A,0x80);
  D_0029DD24=1;
 }
 func_0018D800((const UiStyle *)(void *)(D_003616E0));
 func_00254D68(text,D_0029C4D0,D_0029DD30->x4C,D_004225A0.x12208/20);
 func_0018D930(obj,500,202,text);
}
