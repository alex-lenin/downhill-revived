/* UNIT 0x001B9910 0x001B9940 */
struct Vec001B9910 {
    int x;
    int y;
    int z;
};

struct Obj001B9910 {
    char pad0[0x14C];
    struct Vec001B9910 x14C[1];
};

void func_001B9910(struct Obj001B9910 *obj, int index, int *x, int *y, int *z)
{
    struct Vec001B9910 *v = &obj->x14C[index];

    *x = v->x;
    *y = v->y;
    *z = v->z;
}
