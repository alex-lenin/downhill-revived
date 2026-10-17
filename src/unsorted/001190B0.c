/* UNIT 0x001190B0 0x00119120 */
struct Item { char pad[12]; short xC; };
struct Obj { char pad[0x1004C]; int x1004C; };
extern struct Item *func_001177F8(struct Obj *obj, int index);
short func_001190B0(struct Obj *obj) {
    int i = obj->x1004C - 1;
    while (--i > 0) {
        short value = func_001177F8(obj, i)->xC;
        if (value >= 2) return value;
    }
    return 0;
}
