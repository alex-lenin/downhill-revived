/* UNIT 0x00117828 0x001178A0 */
#include "track_bins.h"

extern void func_001177F8(void *, int);
extern void *func_00118DD8(void *, int);
void *func_00117828(char *obj, int arg1) {
 short indices[2];
 int index;
 func_00117A78((PathBins *)(obj + 0x10070),(float *)(arg1),(short *)(&indices[0]),(short *)(&indices[1]));
 index = indices[0];
 if (index >= 0 || (index = indices[1]) >= 0) func_001177F8(obj, index);
 else return func_00118DD8(obj, arg1);
}
