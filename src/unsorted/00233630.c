/* UNIT 0x00233630 0x00233658 */
struct Obj { char pad0[0x84]; int entries[65]; unsigned int count; };
int func_00233630(struct Obj *obj, unsigned int index) {
    if (index < obj->count) return obj->entries[index];
    return 0;
}
