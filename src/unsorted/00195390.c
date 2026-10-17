/* UNIT 0x00195390 0x001953B8 */
struct Obj { char pad[0x34]; unsigned char x34, x35, x36, x37; };
void func_00195390(struct Obj *obj, unsigned char *x34, unsigned char *x35, unsigned char *x36, unsigned char *x37) {
    *x34 = obj->x34;
    *x35 = obj->x35;
    *x36 = obj->x36;
    *x37 = obj->x37;
}
