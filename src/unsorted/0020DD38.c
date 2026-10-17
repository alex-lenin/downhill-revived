/* UNIT 0x0020DD38 0x0020DE58 */
extern int strlen(const char *s);

void func_0020DD38(char *dst, char *path)
{
    int len;
    int slash;
    int colon;
    int dot;
    int i;
    int n;

    len = strlen(path);
    slash = 0;
    colon = 0;
    dot = len;
    for (i = 0; i < len; i++) {
        if (path[i] == '/' || path[i] == 0x5C) {
            slash = i;
        }
    }
    for (i = 0; i < len; i++) {
        if (path[i] == ':') {
            colon = i;
        }
    }
    for (i = len - 1; i >= 0; i--) {
        if (path[i] == '.') {
            dot = i;
        }
    }
    n = 0;
    for (i = (slash < colon ? colon : slash) + 1; i < dot; i++) {
        dst[n++] = path[i];
    }
    dst[n] = 0;
}
