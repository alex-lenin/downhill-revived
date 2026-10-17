/* UNIT 0x001332A8 0x001332F0 */
extern void func_00232BF8(void *, void *);
extern void func_00134F30(void *, void *);
void func_001332A8(char *dest, char *source) {
    func_00232BF8(dest, source);
    func_00134F30(dest + 0x2E0, source + 0xE0);
    func_00134F30(dest + 0x360, source + 0xE8);
}
