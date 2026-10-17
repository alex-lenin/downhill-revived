/* UNIT 0x00141128 0x00141130 */
struct S_00141128 {
    char pad[0x12C];
    unsigned char x12C;
};

unsigned char func_00141128(struct S_00141128 *s) {
    return s->x12C;
}
