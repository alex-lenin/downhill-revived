/* UNIT 0x001A1258 0x001A1380 */
typedef struct Entry { char pad0[0x694]; } Entry;

extern Entry D_004225A0[];
extern char D_0029D268[];
extern void func_00194D88(Entry *e, int a, unsigned char *d0, unsigned char *d1, unsigned char *d2, unsigned char *d3);
extern int func_00254D68(char *, char *, ...);
extern void func_0020F9A8(int, int, int, int, char *);

void func_001A1258(void *unused, int a, int index, int x, int y0, int y1, int y2, int y3)
{
    char text[0x20];
    unsigned char digits[4];

    func_00194D88(&D_004225A0[index], a, &digits[0], &digits[1], &digits[2], &digits[3]);
    func_00254D68(text, D_0029D268, digits[0]);
    func_0020F9A8(0, 0, y0, x, text);
    func_00254D68(text, D_0029D268, digits[1]);
    func_0020F9A8(0, 0, y1, x, text);
    func_00254D68(text, D_0029D268, digits[2]);
    func_0020F9A8(0, 0, y2, x, text);
    func_00254D68(text, D_0029D268, digits[3]);
    func_0020F9A8(0, 0, y3, x, text);
}
