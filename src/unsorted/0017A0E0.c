/* UNIT 0x0017A0E0 0x0017A1B0 */
#include "format.h"
typedef struct { char pad0[0x528]; int x528; } Global;
typedef struct { char pad0[0x1280]; int x1280; } Obj;
extern Global D_003985B0;
extern char D_0028BC48[];
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);

extern void func_0017B520(Obj *, char *, float, float);
void func_0017A0E0(Obj *obj) {
 char text[32];
 if (D_003985B0.x528 != 0x0FFFFFFF) {
  func_0020FB80(0, 240, 192, -82, -32);
  func_0020F588(obj->x1280, 0, 110, 110, 200, 170);
  FormatHmsDiscard(D_003985B0.x528, (char *)(text));
  func_0017B520(obj, D_0028BC48, 0.2f, 0.4f);
  func_0017B520(obj, text, 0.2f, 0.5f);
 }
}
