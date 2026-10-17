/* UNIT 0x001AF400 0x001AF408 */
struct S_001AF400 {
    char pad[0x14];
    int x14;
};

void func_001AF400(struct S_001AF400 *s) {
    s->x14 = 0;
}
