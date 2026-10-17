/* UNIT 0x00167BC0 0x00167F60 */
extern int D_003985B0[];
extern int D_0026E9F0[], D_0026EA08[], D_0026EA20[], D_0026EA38[], D_0026EA50[];
extern int D_0026EA68[], D_0026EA80[], D_0026EA98[], D_0026EAB0[];
extern char *D_0026EB88[];
extern char D_0028ACB0[], D_0028ACC8[], D_0028ACE8[], D_0028AD10[], D_0028AD30[];
extern char D_0028AD58[], D_0028AD78[], D_0028AD98[], D_0028ADB8[], D_0028ADD0[];
extern char D_0028ADF0[], D_0029C818[], D_0029C820[];

extern void func_0023D650(int, int *, int *);
extern int *func_001696A8(int *, int);
extern int func_001697F0(int *, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, const char *);
extern void func_0020F9A8(int, int, int, int, const char *);
extern void func_00254D68(char *, const char *, ...);

void func_00167BC0(int unused, int font, int y, int showTitle, int alpha, int level) {
    char buf[64];
    int width, height;
    int half;
    int *list;
    int i;

    func_0023D650(font, &width, &height);
    half = width / 2;
    list = func_001696A8(D_003985B0, level);
    if (showTitle) {
        func_0020F518(0, 221, 219, 10, alpha);
        func_0020FB80(0, 352, 256, -139, -32);
        func_0020F9F0(0, font, half, y, D_0028ACB0);
        y += 18;
    }
    func_0020FB80(0, 288, 224, -111, -32);
    for (i = 0; list[i] != 15; i++, y += 12) {
        int type;
        func_0020F518(0, 110, 110, 200, alpha);
        type = list[i];
        if (type == 0) {
            func_00254D68(buf, D_0028ACC8, D_0026E9F0[level]);
        } else if (type == 1) {
            func_00254D68(buf, D_0028ACE8, D_0026EA08[level]);
        } else if (type == 2) {
            func_00254D68(buf, D_0028AD10, D_0026EA38[level]);
        } else if (type == 11) {
            func_00254D68(buf, D_0028AD30, D_0026EA50[level]);
        } else if (type == 13) {
            func_00254D68(buf, D_0028AD58, D_0026EA68[level]);
        } else if (type == 14) {
            func_00254D68(buf, D_0028AD78, D_0026EA80[level]);
        } else if (type == 10) {
            func_00254D68(buf, D_0028AD98, D_0026EA20[level]);
        } else if (type == 16) {
            func_00254D68(buf, D_0028ADB8, D_0026EA98[level]);
        } else if (type == 17) {
            func_00254D68(buf, D_0028ADD0, D_0026EAB0[level]);
        } else {
            func_00254D68(buf, D_0029C818, D_0026EB88[type]);
        }
        func_0020F9A8(0, font, 16, y, buf);
        if (func_001697F0(D_003985B0, list[i])) {
            func_0020F518(0, 88, 152, 73, alpha);
            func_0020F9A8(0, font, 392, y, D_0029C820);
        } else {
            func_0020F518(0, 120, 50, 50, alpha);
            func_0020F9A8(0, font, 392, y, D_0028ADF0);
        }
    }
}
