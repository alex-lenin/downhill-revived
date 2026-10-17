/* UNIT 0x00133260 0x001332A8 */
extern void func_00232B60(void *, void *);
extern void func_00134F00(void *, void *);
void func_00133260(char *dest, char *source) {
    func_00232B60(dest, source);
    func_00134F00(dest + 0x2E0, source + 0xE0);
    func_00134F00(dest + 0x360, source + 0xE8);
}
