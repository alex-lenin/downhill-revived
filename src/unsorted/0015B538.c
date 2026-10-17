/* UNIT 0x0015B538 0x0015B558 */
struct Obj {
    char pad0[0x1E4];
    unsigned short x1E4;
};

void func_0015B538(struct Obj *obj)
{
    int count = obj->x1E4 - 1;

    obj->x1E4 = count;
    if ((short)count <= 0) {
        obj->x1E4 = 0;
    }
}
