/* UNIT 0x0020B880 0x0020B8F8 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0020B908_29_func_0020B908 func_0020B908
#else
#define CallScalarBridge0020B908_29_func_0020B908 ScalarBridge0020B908_29
#endif

#if defined(__mips__)
extern void func_0020B908(void*,char*,unsigned int,float);
#else

#endif

void func_0020B880(void *p,char *q,float f) {
    CallScalarBridge0020B908_29_func_0020B908(p,q,0xff202080U,f);
    CallScalarBridge0020B908_29_func_0020B908(p,q+16,0x20ff2080U,f);
    CallScalarBridge0020B908_29_func_0020B908(p,q+32,0x2020ff80U,f);
}
