/* UNIT 0x001E68A0 0x001E68C0 */
struct Obj {
    char pad0[0x720];
    int x720;
    int x724;
    char pad728[0x10D8 - 0x728];
    int x10D8;
    int x10DC;
    char x10E0;
    char x10E1;
    char pad10E2[2];
    char x10E4;
};

void func_001E68A0(struct Obj *obj)
{
    obj->x10D8 = 0;
    obj->x10DC = 0;
    obj->x724 = 0;
    obj->x720 = 0;
    obj->x10E0 = 0;
    obj->x10E1 = 0;
    obj->x10E4 = 0;
}
