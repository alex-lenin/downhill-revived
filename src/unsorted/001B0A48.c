/* UNIT 0x001B0A48 0x001B0A50 */
struct S_001B0A48 {
    char pad[0x64];
    int x64;
};

void func_001B0A48(struct S_001B0A48 *s) {
    s->x64 = 0;
}
