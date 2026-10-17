/* UNIT 0x0022F5E0 0x0022F600 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Source { char pad[0x800]; u128 x800; };
struct Obj { char pad[0x10]; struct Source *x10; };
struct Buffer { char pad[8]; u128 *x8; };
void func_0022F5E0(struct Obj *obj, struct Buffer *buffer) {
 *buffer->x8 = obj->x10->x800;
 buffer->x8++;
}
