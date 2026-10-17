/* UNIT 0x0013CA48 0x0013CA68 */
#include "scalar_api.h"

#if defined(__mips__)
extern void func_0018D898(void *p, int a, void *b);
#else

#endif


void func_0013CA48(char *obj, int a, void *b)
{
    func_0018D898(obj + 0x504, a, b);
}
