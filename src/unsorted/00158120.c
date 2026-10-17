/* UNIT 0x00158120 0x00158158 */
struct Obj { int values[100]; float weights[100]; int count; };
void func_00158120(struct Obj *obj, int value, float weight) {
    int count = obj->count;
    if (count < 100) {
        obj->weights[count] = weight;
        count = obj->count;
        obj->values[count] = value;
        obj->count = count + 1;
    }
}
