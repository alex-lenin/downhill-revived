/* UNIT 0x001B9860 0x001B9890 */
struct Vec001B9860 {
    int x;
    int y;
    int z;
};

struct Obj001B9860 {
    char pad0[0x11C];
    struct Vec001B9860 x11C[1];
};

void func_001B9860(struct Obj001B9860 *obj, int index, int *x, int *y, int *z)
{
    struct Vec001B9860 *v = &obj->x11C[index];

    *x = v->x;
    *y = v->y;
    *z = v->z;
}
