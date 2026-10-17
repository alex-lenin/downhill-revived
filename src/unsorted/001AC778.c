/* UNIT 0x001AC778 0x001AC780 */
struct S_001AC778 {
    char pad[0x3C];
    float x3C;
};

void func_001AC778(struct S_001AC778 *s, float v) {
    s->x3C = v;
}
