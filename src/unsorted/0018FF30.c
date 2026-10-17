/* UNIT 0x0018FF30 0x0018FF48 */
typedef struct Vec4 {
    float x, y, z;
    int w;
} Vec4;

struct A {
    char pad0[0x10];
    Vec4 x10;
};

void func_0018FF30(struct A *a, float x, float y, float z)
{
    Vec4 *v = &a->x10;
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = 0;
}
