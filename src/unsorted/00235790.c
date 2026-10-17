/* UNIT 0x00235790 0x00235798 */
struct S_00235790 {
    char pad[0x600];
    int x600;
};

void func_00235790(struct S_00235790 *s, int v) {
    s->x600 = v;
}
