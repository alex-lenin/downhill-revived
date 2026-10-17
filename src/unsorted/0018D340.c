/* UNIT 0x0018D340 0x0018D360 */
struct Obj {
    char *x0;
};

extern void func_001AC578(void *p);

void func_0018D340(struct Obj *obj)
{
    func_001AC578(obj->x0 + 0x1E40);
}
