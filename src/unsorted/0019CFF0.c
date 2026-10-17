/* UNIT 0x0019CFF0 0x0019D120 */
#include "ui_region.h"
#if defined(__mips__)
#define CallShared0019C868_0 func_0019C868
#else
#define CallShared0019C868_0 Forward0019C868_0
#endif
#include "menu_items.h"
typedef struct Obj { char p0[0x418]; int x418; char p41c[0x8c]; int x4a8; char p4ac[0x58]; int x504; char p508[0x18]; unsigned char x520; } Obj;
extern int D_0029DE30;
extern char D_003F52D0[], D_0043DD38[];
extern void func_0018E638(Obj *, int, void *);
extern void func_00190358(void *);
extern void func_001B5AF0(void *, int, int);
extern void func_0019D588(Obj *);
extern void func_0019CB08(void *, int, int);
extern void func_0019C868(void *, float, float, float, float, int, int, int, int);
extern void func_0018DC20(Obj *);
void func_0019CFF0(Obj *obj) {
 obj->x418 = 0;
 func_0018E638(obj, 0, &D_0029DE30);
 func_00190358(D_003F52D0);
 if (obj->x520 == 0) {
  func_001B5AF0(D_003F52D0, 5, 0);
  func_0019D588(obj);
 } else {
  func_001B5AF0(D_003F52D0, 5, 1);
  func_0018E638(obj, 62, &D_0029DE30);
  func_0018E3A8((struct SharedMenu *)(void *)(obj), 30, &D_0029DE30, 1);
  func_0018E3A8((struct SharedMenu *)(void *)(obj), 31, &D_0029DE30, 1);
  func_0019CB08(D_0043DD38, 0, 0);
  CallShared0019C868_0(D_0043DD38, 64.0f, 64.0f, 384.0f, 336.0f, 0, 127, 1, 1);
  obj->x504--;
 }
 func_0018DC20(obj);
 obj->x4a8 = 5;
}
