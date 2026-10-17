/* UNIT 0x00212E80 0x00212EA8 */
struct Obj { char pad0[0x28]; float x28; char pad2C[0x10]; float x3C; };
int func_00212E80(struct Obj *obj) {
    return obj->x3C >= obj->x28;
}
