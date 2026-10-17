/* UNIT 0x0020D060 0x0020D078 */
struct Obj {
    char pad0[0x1684];
    int x1684;
    int x1688;
    char pad168C[0x1694 - 0x168C];
    int x1694;
};

struct Obj *func_0020D060(struct Obj *obj)
{
    obj->x1684 = 0;
    obj->x1688 = 0;
    obj->x1694 = 0;
    return obj;
}
