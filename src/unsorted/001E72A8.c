/* UNIT 0x001E72A8 0x001E72F0 */
struct Obj { char pad[0x9BC]; int x9BC[22]; int xA14; };
void func_001E72A8(struct Obj *obj, int flags, int mask) {
    int index;
    flags &= 0xF0F0;
    if (flags != 0) {
        flags &= mask;
        if (flags != 0) {
            index = obj->xA14;
            obj->x9BC[index] = flags;
            index++;
            obj->xA14 = index;
            if (index >= 20) obj->xA14 = 0;
        }
    }
}
