/* UNIT 0x001AF090 0x001AF098 */
struct S_001AF090 {
    char pad[0x18];
    int x18;
};

void func_001AF090(struct S_001AF090 *s) {
    s->x18 = 0;
}
