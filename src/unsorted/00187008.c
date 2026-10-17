/* UNIT 0x00187008 0x00187038 */
#include "load_transition.h"

extern void func_00187038(void *obj);

void *func_00187008(void *obj)
{
    InitializeLoadTransitionDiscard(obj);
    func_00187038(obj);
    return obj;
}
