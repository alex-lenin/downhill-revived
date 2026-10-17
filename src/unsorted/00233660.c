/* UNIT 0x00233660 0x00233670 */
struct Obj {
    char pad0[0x1DC];
    int x1DC;
    int x1E0;
    int x1E4;
};

int func_00233660(struct Obj *obj, int a, int b)
{
    obj->x1DC = a;
    obj->x1E4 = b;
    return 0;
}
