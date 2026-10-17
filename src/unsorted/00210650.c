/* UNIT 0x00210650 0x00210698 */
#include "timer.h"
typedef struct { char pad[0x38]; int x38; } Obj;
extern void func_002106D8(Obj *);
extern void func_00263B08(void);
extern char D_00759780[];
int func_00210650(Obj *obj) {
    func_002106D8(obj);
    obj->x38 = 0;
    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00263B08), obj, 1);
    return 0;
}
