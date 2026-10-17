/* UNIT 0x00208900 0x00208A20 .sdata=0x29D9C0:0x29D9D0 .rodata=0x297C90:0x297CC0 */
typedef struct SoundBankList {
    int *banks[30];
    int count; /* 0x78 */
} SoundBankList;

extern int strlen(const char *);
extern char *strcpy(char *, const char *);
extern unsigned char D_0029B301[]; /* ctype table, bit 1 = lowercase */
extern char *func_002542C4(char *, const char *); /* strcat */
extern void func_00202820(int);
extern void func_0020E520(int *, char *);
extern int func_00201718(int, int);
extern int func_00254D68(char *, const char *, ...); /* sprintf */

void func_00208900(SoundBankList *list, int *bank, char *name) {
    char path[112];
    int file[12];
    char message[64];
    int length;
    int i;

    length = strlen(name);
    strcpy(path, "\\SND\\");
    for (i = 0; i < length; i++) {
        int c = name[i];
        int upper = c - 32;
        if (!(D_0029B301[c] & 2))
            upper = c;
        name[i] = upper;
    }
    func_002542C4(path, name);
    func_002542C4(path, ";1");
    func_00202820(0);
    func_0020E520(file, path);
    *bank = func_00201718(file[0], 0);
    func_00254D68(message, "Failure Loading Sound Bank! (%s)\n", path);
    list->banks[list->count] = bank;
    list->count++;
}
