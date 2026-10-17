/* UNIT 0x001615F0 0x00161600 */
struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
};

void func_001615F0(struct Obj *obj)
{
    obj->x500 = 0;
    obj->x504 = 0;
}
