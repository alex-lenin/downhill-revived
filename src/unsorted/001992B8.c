/* UNIT 0x001992B8 0x001992E0 */
struct Obj { char pad[0x204C]; int x204C; };
void *func_001992B8(struct Obj *obj, int index) {
    if (index < 0 || index >= obj->x204C) {
        index = 0;
    }
    return (char *)obj + (index << 5);
}
