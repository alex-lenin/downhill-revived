/* UNIT 0x0012DE00 0x0012DE30 */
struct Obj { char pad[0x794C]; unsigned int *x794C; unsigned int *x7950; };
void func_0012DE00(struct Obj *obj, unsigned int *data) {
    int kind = (*data >> 18) & 31;
    if (kind == 22) {
        obj->x7950 = data;
    } else if (kind == 23) {
        obj->x794C = data;
    }
}
