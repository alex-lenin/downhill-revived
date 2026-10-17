/* UNIT 0x0020FBB0 0x0020FDB0 .rodata=0x298220:0x298290 */
typedef struct {
    char pad0[0xC];
    unsigned short xC, xE, x10, x12, x14;
    char pad16[6];
    int x1C;
} Font;

extern Font D_004F3570[];
extern int func_0020F860(int, char *);
extern int func_0020FDB0(int, int, int);

void func_0020FBB0(int index, int arg1, char *s)
{
    Font *font = &D_004F3570[index];
    int adjust = 0;

    while (*s != 0) {
        switch (*s) {
        case ' ':
            font->x10 += font->xE - adjust;
            adjust = 0;
            break;
        case '\b':
            font->x10 -= font->xE;
            adjust = 0;
            break;
        case '\t': {
            int tab = font->xE * 4;
            if (font->x1C == 0) {
                font->x10 += tab - (font->x10 - font->x14) % tab;
            } else {
                font->x10 += tab;
            }
            adjust = 0;
            break;
        }
        case '\n':
            font->x12 += font->xC;
            /* fall through */
        case '\r':
            switch (font->x1C) {
            case 0:
                font->x10 = font->x14;
                break;
            case 1:
                font->x10 = font->x14 - (func_0020F860(index, s + 1) >> 1);
                break;
            case 2:
                font->x10 = font->x14 - func_0020F860(index, s + 1);
                break;
            }
            adjust = 0;
            break;
        case '\v':
            font->x12 += font->xC;
            adjust = 0;
            break;
        default:
            adjust = func_0020FDB0(index, arg1, (unsigned char)*s);
            break;
        }
        s++;
    }
    font->x1C = 0;
}
