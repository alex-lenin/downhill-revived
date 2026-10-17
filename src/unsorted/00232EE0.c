/* UNIT 0x00232EE0 0x00232F78 */
extern int strlen(const char *);
extern unsigned int func_00232DF0(unsigned char c);

void func_00232EE0(unsigned char *text, unsigned short *out) {
    int i;
    int length;
    unsigned int value;
    unsigned char hi;
    unsigned char lo;

    length = strlen((char *)text);
    for (i = 0; i < length; i++) {
        value = func_00232DF0(text[i]);
        lo = value;
        hi = value >> 8;
        out[i] = hi | (lo << 8);
    }
    out[i] = 0;
}
