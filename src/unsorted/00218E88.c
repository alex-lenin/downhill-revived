/* UNIT 0x00218E88 0x00218FA8 */
typedef struct TexFileList {
    char pad0[0x1680];
    int count;
} TexFileList;

extern TexFileList D_004D30B0;
extern char D_00298430[];
extern int D_0029E21C;
extern int D_0029E220;
extern int D_0029E25C;

extern int func_0020D170(void *list, int index);
extern void func_002189F8(char *format, ...);
extern int func_0021AFF8(int file);
extern void func_0021B020(int file, int base);
extern void func_00218FA8(int arg);

int func_00218E88(int base, int arg1)
{
    int i;
    int relocated;
    int count;

    relocated = 0;
    for (i = 0; i < D_004D30B0.count; i++) {
        count = D_004D30B0.count;
        func_002189F8(D_00298430, i + 1, count, func_0020D170(&D_004D30B0, i));
        if (func_0021AFF8(func_0020D170(&D_004D30B0, i)) != 0) {
            relocated = 1;
            func_0021B020(func_0020D170(&D_004D30B0, i), base);
        }
    }
    D_0029E25C = base;
    D_0029E21C = base;
    D_0029E220 = base;
    if (relocated) {
        func_00218FA8(arg1);
    }
    return D_0029E21C;
}
