/* UNIT 0x001AE998 0x001AE9B0 */
struct Vec3 {
    float x;
    float y;
    float z;
};

struct Unk001AE998 {
    char pad0[0x10];
    struct Vec3 x10;
};

void func_001AE998(struct Unk001AE998 *p, float x, float y, float z)
{
    struct Vec3 *v = &p->x10;

    v->x = x;
    v->y = y;
    v->z = z;
}
