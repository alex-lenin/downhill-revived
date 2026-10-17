/* UNIT 0x00157658 0x00157698 */
struct Obj { char *x0; char pad[0xE1C]; };
extern void func_001577F8(void *, void *);
extern void func_001A4C50(void *, void *);
void func_00157658(struct Obj *obj) {
    func_001577F8((char *)obj + 0xE20, obj->x0);
    func_001A4C50((char *)obj + 0x10, obj->x0 + 0x1E40);
}
