/* UNIT 0x00112878 0x001128A8 */
struct Obj { char pad[0x3D4]; int x3D4[10]; int x3FC; };
void func_00112878(struct Obj *obj, int index, int amount) {
    int *value;
    int result;
    int minimum;
    value = obj->x3D4;
    value += index;
    result = *value - amount;
    *value = result;
    minimum = obj->x3FC;
    *value = minimum >= result ? minimum : result;
}
