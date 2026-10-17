/* UNIT 0x00198140 0x00198168 */
struct Obj { char pad[0x500]; int x500; };
extern char D_004348F0[];
extern void func_00196410(void *obj, int value);
void func_00198140(struct Obj *obj) {
    func_00196410(D_004348F0, obj->x500);
}
