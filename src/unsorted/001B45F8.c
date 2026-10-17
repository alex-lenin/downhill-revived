/* UNIT 0x001B45F8 0x001B4618 */
struct Obj {
    int x0;
    int active;
    int x8;
    int xC;
};

void func_001B45F8(struct Obj *obj, int c, int b)
{
    if (obj->active == 0) {
        obj->active = 1;
        obj->xC = c;
        obj->x8 = b;
    }
}
