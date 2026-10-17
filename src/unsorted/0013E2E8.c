/* UNIT 0x0013E2E8 0x0013E498 .rodata=0x289A98:0x289AA8 */
#include "ui_style.h"
typedef struct Entry { char pad0[6]; unsigned char x6; char pad7[9]; } Entry;
typedef struct Obj { char pad0[0x10]; void *x10; char pad14[4]; Entry x18[64]; int x418; char pad41C[0x44]; int x460,x464; char pad468[0xA8]; int x510; } Obj;
typedef struct Global { char pad0[0x70]; int x70; } Global;
typedef struct IdList { int codes[3]; } IdList;
const IdList D_00289A98 = {{0x259B,0x259C,0x259A}};
extern int D_004B26AC[];
extern int D_00364D28[],D_003F52F0[];
extern char D_00366800[],D_003674F0[],D_003681E0[];
extern Global *D_0029DD30;
extern void func_0013DCF8(Obj *);
extern int func_0018D8B8(void *);
extern void *func_0013AF60(int);
extern void func_00218498(void *,int);
extern void func_0020FA98(int,int,int,int,int);
extern void func_0018DFC8(Obj *,int);
extern void func_0018DBA8(Obj *);
void func_0013E2E8(Obj *obj) {
 func_0013DCF8(obj);
 {
 int mode=D_004B26AC[0];
 IdList ids=D_00289A98;
 int found,i,count,one; Entry *entry;
 if(mode==3 || mode==0 || mode==1 || mode==2) {
  if(D_00364D28[0] || func_0018D8B8(D_00366800) || func_0018D8B8(D_003674F0) || func_0018D8B8(D_003681E0)) goto active;
 }
  func_00218498(func_0013AF60(0x259B),0);
  func_00218498(func_0013AF60(0x259C),0);
  func_00218498(func_0013AF60(0x259A),0);
  func_0018D800((const UiStyle *)(void *)(obj->x10));
  func_0020FA98(0,D_003F52F0[0],obj->x464,obj->x460,D_0029DD30->x70);
  goto end;
active:
  func_0013DCF8(obj);
  found=0;
  i=0;
  if(obj->x418<=0) goto no_entries;
   one=1;
   entry=obj->x18;
   do {
    if(entry->x6) found=one;
    i++;
    entry++;
   } while(i<obj->x418 && found==0);
no_entries:
  func_00218498(func_0013AF60(ids.codes[obj->x510-1]),found);
end:;
 }
 func_0018DFC8(obj,1); func_0018DBA8(obj);
}
