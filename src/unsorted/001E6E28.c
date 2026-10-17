/* UNIT 0x001E6E28 0x001E6E40 */
struct Obj {
    char pad0[0x10E4];
    unsigned char x10E4;
    char pad10E5[0x10EC - 0x10E5];
    int x10EC;
};

int func_001E6E28(struct Obj *obj)
{
    int result = obj->x10E4;

    if (obj->x10EC != 0) {
        result = 1;
    }
    return result;
}
