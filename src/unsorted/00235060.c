/* UNIT 0x00235060 0x002350D0 */
extern int func_00222B08(int, int);
int func_00235060(void) {
    int value = func_00222B08(0x80000001, 0x7fffffff);
    if (!(value & 0xffff0000)) {
        value = (value << 16) | func_00222B08(0x80000001, 0x7fffffff);
        value &= 0x7fffffff;
    }
    return value;
}
