/* UNIT 0x001F97A8 0x001F9808 */
#include "scalar_api.h"
typedef struct Scalar001F97A8Obj { char pad[0x88]; unsigned long x88; unsigned int x90, x94; } Obj;
#if defined(__mips__)
#define FontBitTestResult long
#else
#define FontBitTestResult int
#endif
FontBitTestResult func_001F97A8(Obj *p, int kind, int bit) {
    switch (kind) {
    case 1: return ((1L << bit) & p->x88) != 0;
    case 2: return ((long)p->x90 >> bit) & 1;
    case 3: return ((long)p->x94 >> bit) & 1;
    default: return 0;
    }
}
