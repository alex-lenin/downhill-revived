/* UNIT 0x001577B0 0x001577B8 */
struct S_001577B0 {
    char pad[0xDB0];
    unsigned char xDB0;
};

unsigned char func_001577B0(struct S_001577B0 *s) {
    return s->xDB0;
}
