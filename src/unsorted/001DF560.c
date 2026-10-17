/* UNIT 0x001DF560 0x001DF600 .sdata=0x29D678:0x29D690 */
extern int func_00247A40(int);
extern char *strcpy(char *, const char *);
extern char *strncat(char *, const char *, unsigned int);
extern int func_0020E520(int *, const char *);
extern void func_0025F298(int);

void func_001DF560(int unused, const char *name, int *out)
{
    int result[12];
    char path[48];

    func_00247A40(0);
    strcpy(path, "\\VAG\\");
    strncat(path, name, 40);
    strncat(path, ";1", 40);
    if (!func_0020E520(result, path)) {
        func_0025F298(-1);
    }
    *out = result[0];
}

char D_0029D688[] __attribute__((aligned(8))) = "\n";
