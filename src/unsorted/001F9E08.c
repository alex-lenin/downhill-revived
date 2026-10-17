/* UNIT 0x001F9E08 0x001F9E50 */
extern void func_001F9C98(int);
void func_001F9E08(int p) {
    int end = p + 0x3c0;
    do {
        func_001F9C98(p);
        p += 0xa0;
    } while (p < end);
}
