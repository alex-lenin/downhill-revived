/* UNIT 0x001AFB88 0x001AFBA8 */
struct Obj {
    char pad0[0x50];
    int x50;
    char pad54[0xE4 - 0x54];
    int xE4;
    int xE8;
    int xEC;
    int xF0;
    char xF4;
};

void func_001AFB88(struct Obj *obj)
{
    obj->xF4 = 1;
    obj->x50 = 0;
    obj->xE8 = 0;
    obj->xF0 = 0;
    obj->xE4 = 0;
    obj->xEC = 0;
}
