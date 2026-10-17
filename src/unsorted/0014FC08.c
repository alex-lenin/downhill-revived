/* UNIT 0x0014FC08 0x0014FC18 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Obj { char pad[0x490]; u128 x490; };
void func_0014FC08(struct Obj *obj) {
    obj->x490 = 0;
}
