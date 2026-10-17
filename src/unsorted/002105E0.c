/* UNIT 0x002105E0 0x00210650 */
#include "timer.h"
extern void func_002106C8(void *);
extern void func_00210738(void *,int,int);
extern void func_00210760(void *);
extern char D_00759780[];
extern void func_00263AE8(void);
void func_002105E0(void *obj,int a,int b) {
    func_002106C8(obj);
    func_00210738(obj,a,b);
    func_00210760(obj);
    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00263AE8), obj, 1);
}
