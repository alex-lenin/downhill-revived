/* UNIT 0x001177F8 0x00117828 */
struct Obj { char pad[0x1004C]; int x1004C; };
void *func_001177F8(struct Obj *obj, int index) {
    if (index < 0 || index >= obj->x1004C) {
        index = 0;
    }
    return (char *)obj + (index << 5);
}
