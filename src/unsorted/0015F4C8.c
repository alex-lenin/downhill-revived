/* UNIT 0x0015F4C8 0x0015F5D0 */
extern char D_0029C770[]; /* "%d" */
extern unsigned int strlen(const char *s);
extern int func_00254D68(char *buf, const char *fmt, int value);

/* Overwrites the last 4 characters of text with value * 0.78125, right-aligned. */
void func_0015F4C8(void *unused, char *text, int unused2, int value)
{
    char buf[16];
    unsigned int len;
    int digits;
    int i;
    int n;
    int width;

    len = strlen(text);
    for (n = 3; n >= 0; n--) {
        text[len - 4 + n] = ' ';
    }
    value = value * 0.78125f;
    func_00254D68(buf, D_0029C770, value);
    if (value >= 1000) {
        digits = 4;
    } else if (value >= 100) {
        digits = 3;
    } else if (value >= 10) {
        digits = 2;
    } else {
        digits = 1;
    }
    for (i = 0; i < digits; i++) {
        width = digits + 1;
        text[len - width + i] = buf[i];
    }
}
