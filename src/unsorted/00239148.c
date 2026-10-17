/* UNIT 0x00239148 0x00239178 */
struct Obj { char pad[0x2838]; unsigned int *x2838; char pad283C[0x14]; int x2850; };
void func_00239148(struct Obj *obj) {
    unsigned int *entry = (unsigned int *)((char *)obj->x2838 + (obj->x2850 - 1) * 4);
    *entry &= 0xFFFFFF;
}
