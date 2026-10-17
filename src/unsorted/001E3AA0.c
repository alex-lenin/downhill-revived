/* UNIT 0x001E3AA0 0x001E3CD0 */
extern void func_0023D650(int, int *, int *);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, void *);
extern void func_0020F518(int, int, int, int, int);
extern int D_004C5B40[];
extern char D_00294700[];
extern void *D_00271878[];
extern void *D_002718B8[];
extern void *D_002718F8[];
extern void *D_00271938[];
extern void *D_00271978[];
extern void *D_002719B8[];

void func_001E3AA0(int unused, int font, int y, int header, int detail, int alpha)
{
    int width, height;
    int x;
    int language;

    func_0023D650(font, &width, &height);
    language = D_004C5B40[0];
    x = width / 2;
    if (header) {
        func_0020F588(font, 0, 221, 219, 10, alpha);
        func_0020FB80(0, 352, 256, -119, -32);
        func_0020F9F0(0, font, x, y, D_00294700);
        y += 18;
    }
    func_0020FB80(0, 288, 224, -91, -32);
    func_0020F9F0(0, font, x, y, D_00271878[language]);
    func_0020F518(0, 110, 110, 200, alpha);
    if (detail) {
        y += 20;
        func_0020FB80(0, 256, 224, -91, -32);
        func_0020F9F0(0, font, x, y, D_002718B8[language]);
        y += 50;
        func_0020FB80(0, 272, 224, -104, -32);
        func_0020F9F0(0, font, x, y, D_002718F8[language]);
        y += 10;
        func_0020F9F0(0, font, x, y, D_00271938[language]);
        y += 10;
        func_0020F9F0(0, font, x, y, D_00271978[language]);
        func_0020F9F0(0, font, x, y + 10, D_002719B8[language]);
    }
}
