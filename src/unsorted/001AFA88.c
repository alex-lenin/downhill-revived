/* UNIT 0x001AFA88 0x001AFAA8 */
struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0xE4 - 0xC];
    int xE4;
    int xE8;
    int xEC;
    int xF0;
    char xF4;
};

void func_001AFA88(struct Obj *obj)
{
    obj->xF4 = 1;
    obj->x8 = 0;
    obj->xE8 = 0;
    obj->xF0 = 0;
    obj->xE4 = 0;
    obj->xEC = 0;
}
