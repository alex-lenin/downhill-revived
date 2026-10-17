/* UNIT 0x00212488 0x00212500 */
#include "spatial.h"
void func_00212488(SpatialReference *obj, const Matrix4 *value, Matrix4 *out, float t)
{
    Vector4 temp;
    Matrix4 matrix;
    if (obj->node) {
        func_002219A8(out, value, obj->node->matrix);
        return;
    }
    if (obj->actor) {
        func_00209460(obj->actor, &temp, &matrix, t);
        func_002219A8(out, value, &matrix);
    }
}
