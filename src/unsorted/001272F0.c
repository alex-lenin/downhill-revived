/* UNIT 0x001272F0 0x001272F8 */
struct S_001272F0 {
    char pad[0x15];
    unsigned char x15;
};

unsigned char func_001272F0(int a0, struct S_001272F0 *s) {
    return s->x15;
}
