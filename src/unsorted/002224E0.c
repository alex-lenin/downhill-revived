/* UNIT 0x002224E0 0x00222578 */
#include "vector.h"
extern float func_00248C90(float);
extern float func_00248D90(float,float);
void func_002224E0(const Matrix4 *matrix, Vector3 *out) {
    out->y = -func_00248C90(matrix->row[0].z);
    if (__builtin_fabsf(matrix->row[0].z) < 0.99999f) {
        out->x = func_00248D90(matrix->row[1].z,matrix->row[2].z);
        out->z = func_00248D90(matrix->row[0].y,matrix->row[0].x);
    } else {
        out->x = func_00248D90(matrix->row[1].x,matrix->row[2].x);
        out->z = 0.0f;
    }
}
