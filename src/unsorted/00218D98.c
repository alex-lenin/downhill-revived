/* UNIT 0x00218D98 0x00218E88 .rodata=0x298400:0x298430 */
typedef struct TextureBank {
    char pad0[0x1680];
    int x1680;
} TextureBank;

extern int D_0029E22C;
extern int D_0029E218;
extern TextureBank D_004D1A18;
extern void func_002189F8(const char *fmt, ...);
extern void *func_0020D170(TextureBank *bank, int index);
extern int func_0021A5C8(void *texture);
extern void func_002197F0(void *texture);

void func_00218D98(int size) {
    int i;

    if (size == 0xC8840) {
        D_0029E22C = 0xFFFF0;
    } else if (size == 0x69840) {
        D_0029E22C = 0xEE7F0;
    } else {
        D_0029E22C = 0xF97F0;
    }
    for (i = 0; i < D_004D1A18.x1680; i++) {
        func_002189F8("About to relocate texture file #%i of %i\n", i + 1, D_004D1A18.x1680);
        func_0021A5C8(func_0020D170(&D_004D1A18, i));
    }
    D_0029E218 = 0;
    func_002197F0(func_0020D170(&D_004D1A18, 0));
}
