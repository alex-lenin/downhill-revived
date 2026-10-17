/* UNIT 0x001E39A0 0x001E3A10 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge001B5AF0_9_func_001B5AF0 func_001B5AF0
#else
#define CallScalarBridge001B5AF0_9_func_001B5AF0 ScalarBridge001B5AF0_9
#endif
struct Obj { char pad[0x5C]; int x5C; };
extern struct Obj D_003F52D0;
extern int D_0029DE68;
extern void func_00220550();

#if defined(__mips__)
extern void func_001B5AF0(void *, long, long);
#else

#endif

extern void func_0018E6B8();
extern void func_0018DB08();
void func_001E39A0(void *obj) {
    func_00220550(0);
    CallScalarBridge001B5AF0_9_func_001B5AF0(&D_003F52D0, 1, 1);
    D_003F52D0.x5C = 100;
    func_0018E6B8(obj, 0x99, &D_0029DE68);
    func_0018DB08(obj);
}
