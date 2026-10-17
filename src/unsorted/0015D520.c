/* UNIT 0x0015D520 0x0015D578 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001B5AF0_9_func_001B5AF0 func_001B5AF0
#else
#define CallScalarBridge001B5AF0_9_func_001B5AF0 ScalarBridge001B5AF0_9
#endif
struct Obj { char pad0[0x460]; int x460; char pad464[0x44]; int x4A8; };
extern char D_003F52D0[]; extern int D_0029DE68;

#if defined(__mips__)
extern void func_001B5AF0(void *, long, long);
#else

#endif

extern void func_0018E6B8();
void func_0015D520(struct Obj *obj) {
    CallScalarBridge001B5AF0_9_func_001B5AF0(D_003F52D0, 1, 1);
    obj->x460 = 40;
    func_0018E6B8(obj, 16, &D_0029DE68);
    obj->x4A8 = 5;
}


