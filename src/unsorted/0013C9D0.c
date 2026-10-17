/* UNIT 0x0013C9D0 0x0013CA10 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0013CA48_4_func_0013CA48 func_0013CA48
#else
#define CallScalarBridge0013CA48_4_func_0013CA48 ScalarBridge0013CA48_4
#endif
typedef struct Obj {
    char pad0[0x524];
    int x524;
    int x528;
} Obj;

extern long D_0029DE68;

#if defined(__mips__)
extern void func_0013CA48(Obj *obj, long arg, long *name);
#else

#endif

extern void func_0018E398(Obj *obj);

void func_0013C9D0(Obj *obj) {
    obj->x524 = 0;
    obj->x528 = 0;
    CallScalarBridge0013CA48_4_func_0013CA48(obj, 0, &D_0029DE68);
    func_0018E398(obj);
}
