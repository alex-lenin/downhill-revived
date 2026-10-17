/* UNIT 0x00230618 0x00230648 */
struct Obj { char pad[0x50000]; int x50000, x50004, x50008; };
int func_00230618(struct Obj *obj, void **out) {
    int count = obj->x50008 - obj->x50004;
    if (count) *out = (char *)obj + obj->x50000;
    return count;
}
