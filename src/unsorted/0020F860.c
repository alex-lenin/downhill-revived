/* UNIT 0x0020F860 0x0020F9A8 */
typedef struct Font {
    char pad0[0xE];
    unsigned short xE; /* character spacing */
    char pad10[0x1C - 0x10];
    int x1C; /* nonzero: tabs are a fixed width */
} Font;

extern Font D_004F3570[];

extern int func_0020F6C0(unsigned char c, int *adjust);

int func_0020F860(int fontIndex, char *text)
{
    int width = 0;
    Font *font = &D_004F3570[fontIndex];
    int tab = font->xE * 4;
    int adjust = 0;

    while (*text != '\n' && *text != '\0') {
        char c = *text;

        if (c == ' ') {
            width += D_004F3570[fontIndex].xE - adjust;
        } else if (c == '\t') {
            if (!font->x1C) {
                width += tab - width % tab;
            } else {
                width += tab;
            }
        } else if (c == '\b') {
            width -= D_004F3570[fontIndex].xE * 2;
        } else {
            width += D_004F3570[fontIndex].xE + func_0020F6C0(*text, &adjust) * 2;
        }
        text++;
    }
    return width;
}
