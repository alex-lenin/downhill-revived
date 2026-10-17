/* UNIT 0x0020DE58 0x0020DF40 */
extern int strlen(const char *);

/* Copies the text between the first path separator and the last separator/colon. */
void func_0020DE58(char *dst, char *path)
{
    int len = strlen(path);
    int first = 0;
    int last = len;
    int i;
    int n;

    for (i = 0; i < len; i++) {
        if (path[i] == '/' || path[i] == 0x5C) {
            first = i;
            break;
        }
    }
    for (i = len - 1; i >= 0; i--) {
        if (path[i] == '/' || path[i] == 0x5C || path[i] == ':') {
            last = i;
            break;
        }
    }
    n = 0;
    for (i = first + 1; i < last; i++) {
        dst[n++] = path[i];
    }
    dst[n] = 0;
}
