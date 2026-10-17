/* UNIT 0x001B0AA0 0x001B0AC0 */
struct Obj {
    char pad0[0x44];
    int x44;
    char pad48[0x68 - 0x48];
    char x68;
    char pad69[3];
    int x6C;
    int x70;
    char x74;
};

void func_001B0AA0(struct Obj *obj)
{
    obj->x68 = 1;
    obj->x74 = 1;
    obj->x6C = 0;
    obj->x70 = 0;
    obj->x44 = 0;
}
