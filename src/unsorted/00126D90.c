/* UNIT 0x00126D90 0x00126DB8 */
#include "scalar_api.h"
extern void func_002087C0(float *);
float func_00126D90(float a, float b) {
    float value = a - b;
    func_002087C0(&value);
    return value;
}
