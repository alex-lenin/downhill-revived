/* UNIT 0x0018FF48 0x0018FF80 */
struct Obj {
    char pad0[0x134];
    int x134;
    char pad138[0x14C - 0x138];
    int x14C;
    char pad150[0x154 - 0x150];
    int x154[12];
};

void func_0018FF48(struct Obj *obj)
{
    int i;

    obj->x134 = 0;
    obj->x14C = 0;
    for (i = 11; i >= 0; i--) {
        obj->x154[i] = 0;
    }
}
