/* UNIT 0x001FACD8 0x001FACE0 */
struct S_001FACD8 {
    char pad[0x2C];
    int x2C;
};

void func_001FACD8(struct S_001FACD8 *s, int v) {
    s->x2C = v;
}
