/* UNIT 0x001DBC58 0x001DBC90 */
struct Obj { int x0; void *x4; };
extern char D_00287FC0[];
extern void func_001DBCC0(struct Obj *);
struct Obj *func_001DBC58(struct Obj *obj) {
    obj->x0 = 0;
    obj->x4 = D_00287FC0;
    func_001DBCC0(obj);
    return obj;
}
