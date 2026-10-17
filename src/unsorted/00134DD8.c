/* UNIT 0x00134DD8 0x00134DE8 */
typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

void func_00134DD8(Vec3 *v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}
