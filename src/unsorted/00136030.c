/* UNIT 0x00136030 0x00136038 */
struct S_00136030 {
    char pad[0x4AF];
    unsigned char x4AF;
};

void func_00136030(struct S_00136030 *s, unsigned char v) {
    s->x4AF = v;
}
