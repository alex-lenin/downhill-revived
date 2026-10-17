/* UNIT 0x0020D250 0x0020D260 */
struct Obj {
    char pad0[0x1684];
    int x1684;
    int x1688;
};

void func_0020D250(struct Obj *obj, int value)
{
    obj->x1684 = 0;
    obj->x1688 = value;
}
