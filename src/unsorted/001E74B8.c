/* UNIT 0x001E74B8 0x001E7508 */
extern void func_001E6498(void *p);
extern void func_001E6478(void *p);
extern void func_001E61E0(void *p);

void func_001E74B8(char **arg0)
{
    char *base = *arg0;

    if (*(char **)(base + 0x4ECC) != base + 0x5FF0) {
        func_001E6498(base + 0x6610);
    }
    func_001E6478(base + 0x6610);
    func_001E61E0(base + 0x6610);
}
