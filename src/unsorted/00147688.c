/* UNIT 0x00147688 0x001476F0 */
struct Obj { char pad[0x1E0]; int x1E0; };
extern void func_00147760(void *, int *, int, int, int);
void func_00147688(struct Obj *obj, int *flags, int arg2, int arg3) {
    int count = obj->x1E0;
    func_00147760((char *)obj + count * 16, flags, count, arg2, arg3);
    if ((*flags & 0x3F) == 6) obj->x1E0++;
}
