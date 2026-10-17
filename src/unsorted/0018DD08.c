/* UNIT 0x0018DD08 0x0018DD80 */
extern int func_0018DF28(int, int);
extern void func_00190108(void *, void *);
extern char D_003F52D0[], D_0038B6F0[];
void func_0018DD08(int count) {
    int i, success = 1;
    for (i = 0; i < count; i++) {
        if (func_0018DF28(i, 1)) success = 0;
    }
    if (!success) func_00190108(D_003F52D0, D_0038B6F0);
}
