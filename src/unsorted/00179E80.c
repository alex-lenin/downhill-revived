/* UNIT 0x00179E80 0x00179FA8 .rodata=0x28BC38:0x28BC48 */
#include "format.h"
typedef struct { char pad0[0x18]; int x18; char pad1C[0x14]; int x30; char pad34[0x4]; int x38; } Record;
typedef struct { char pad0[0x1280]; int x1280; } Obj;
extern Record D_004C3740;
extern char D_0028BC48[]; /* "Best Time", shared with func_0017A0E0 */
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);

extern void func_0017B520(Obj *, char *, float, float);

void func_00179E80(Obj *obj)
{
    char text[32];

    func_0020FB80(0, 240, 192, -82, -32);
    func_0020F588(obj->x1280, 0, 110, 110, 200, 170);
    if (D_004C3740.x38 == 1) {
        FormatHmsDiscard(D_004C3740.x18, (char *)(text));
        func_0017B520(obj, "Incog Best", 0.2f, 0.4f);
        func_0017B520(obj, text, 0.2f, 0.5f);
    } else if (D_004C3740.x30 != 0x0FFFFFFF) {
        FormatHmsDiscard(D_004C3740.x30, (char *)(text));
        func_0017B520(obj, D_0028BC48, 0.2f, 0.4f);
        func_0017B520(obj, text, 0.2f, 0.5f);
    }
}
