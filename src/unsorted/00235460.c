/* UNIT 0x00235460 0x00235488 */
struct Obj { char pad0[0x5FC]; void *x5FC; };
extern char D_00288818[];
extern void func_00233048(struct Obj *obj, int flags);
void func_00235460(struct Obj *obj, int flags) {
    obj->x5FC = D_00288818;
    func_00233048(obj, flags);
}
