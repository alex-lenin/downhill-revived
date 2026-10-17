/* UNIT 0x001B1098 0x001B1100 */
extern void func_001B12A0(int);
#include "rider.h"
void func_001B1098(int base) {
    int p = base;
    do {
        func_001B12A0(p);
        p += 0x110;
    } while (p < base + 0x440);
    func_0023AEB8((*(RiderComponent **)(base + 0x440))->rider->riderIndex);
}
