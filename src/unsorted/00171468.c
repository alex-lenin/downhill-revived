/* UNIT 0x00171468 0x001714C0 */
struct Obj { char pad0[0x2940]; int x2940[31]; int count; };
extern int func_001591D8(int);
void func_00171468(struct Obj *obj, int value) {
    if (obj->count < 20) { obj->x2940[obj->count] = func_001591D8(value); obj->count++; }
}
