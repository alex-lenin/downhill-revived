/* UNIT 0x0023D5C0 0x0023D5F0 */
struct Obj { char pad[0x1B0]; unsigned char x1B0, x1B1, x1B2, x1B3; };
extern struct Obj D_0075BAC0;
void func_0023D5C0(int *a, int *b, int *c, int *d) {
    *a = D_0075BAC0.x1B0;
    *b = D_0075BAC0.x1B1;
    *c = D_0075BAC0.x1B2;
    *d = D_0075BAC0.x1B3;
}
