/* UNIT 0x00119790 0x00119810 */
#include "track_bins.h"

extern void func_001177F8(void *, short);
extern int func_00118DD8(void *, int);
int func_00119790(void *obj, int arg) {
    short first;
    unsigned short second;
    short index;
    func_00117A78((PathBins *)((char *)obj + 0x10070),(float *)(arg),(short *)(&first),(short *)(&second));
    index = (short)second;
    if (index >= 0 || (index = first, index >= 0)) func_001177F8(obj, index);
    else return func_00118DD8(obj, arg);
}
