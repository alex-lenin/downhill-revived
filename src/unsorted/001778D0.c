/* UNIT 0x001778D0 0x001778E8 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Obj { u128 x0; char pad10[0xc70]; void *xC80; };
void func_001778D0(struct Obj *obj) {
    obj->x0 = 0;
    obj->xC80 = (char *)obj + 0x10;
}
