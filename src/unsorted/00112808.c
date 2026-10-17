/* UNIT 0x00112808 0x00112878 */
struct Obj { char pad0[0x3C4]; int x3C4[13]; int x3F8; };
void func_00112808(struct Obj *obj, int index, int amount) {
    int limit, current;
    int value = obj->x3C4[index];
    if (value) {
        if (value < 5) obj->x3C4[index] = value / 2;
        else {
            value -= amount;
            obj->x3C4[index] = value > 3 ? value : 4;
        }
        limit = obj->x3F8;
        current = obj->x3C4[index];
        obj->x3C4[index] = limit >= current ? limit : current;
    }
}
