/* UNIT 0x0020DC28 0x0020DD38 */
#include "disc_path.h"
extern char D_00298128[]; /* "%%s\%%s%s%%s.%%s;1" */
extern char D_0029DA20[]; /* "" */
extern char D_0029DA28[]; /* "\\" */
extern unsigned char D_0029B301[]; /* ctype table, bit 1 = lower case */
extern int func_00254D68(char *buf, const char *fmt, ...);
extern unsigned int strlen(const char *s);

static inline int toupper(int c)
{
    return (D_0029B301[c] & 2) ? c - 'a' + 'A' : c;
}

/* Builds an upper-case CD path "<empty>\<dir>\<name>.<ext>;1" into out. */
void func_0020DC28(char *out, const char *dir, const char *name, const char *ext)
{
    char empty[16];
    char fmt[32];
    char *p;
    unsigned int i;
    int c;
    int noDir;

    noDir = (dir == 0 || *dir == 0);
    func_00254D68(fmt, D_00298128, noDir ? D_0029DA20 : D_0029DA28);
    empty[0] = 0;
    func_00254D68(out, fmt, empty, dir, name, ext);
    for (i = 0, p = out; i < strlen(out); i++) {
        c = *p;
        *p++ = toupper(c);
    }
}
