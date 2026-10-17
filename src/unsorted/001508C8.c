/* UNIT 0x001508C8 0x001508D8 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Obj { char pad[0x490]; u128 x490; };
void func_001508C8(struct Obj *obj) { obj->x490 = 0; }
