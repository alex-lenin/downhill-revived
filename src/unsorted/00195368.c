/* UNIT 0x00195368 0x00195390 */
struct Obj { char pad[0x38]; unsigned short x38, x3A; int x3C, x40; };
void func_00195368(struct Obj *obj, unsigned short *x38, unsigned short *x3A, int *x3C, int *x40) {
    *x38 = obj->x38;
    *x3A = obj->x3A;
    *x3C = obj->x3C;
    *x40 = obj->x40;
}
