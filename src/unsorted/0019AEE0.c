/* UNIT 0x0019AEE0 0x0019AF60 */
extern unsigned char D_004225A0[];
void func_0019AEE0(unsigned char *obj) {
    int j, i, off, marker, next;
    unsigned char *table, *dst, *src, *srcBase;
    table = D_004225A0;
    srcBase = obj + 0x50C;
    i = 0;
    do {
        j = 0;
        next = i + 1;
        off = i * 12 + 0x121D7;
        dst = (unsigned char *)(off + (unsigned int)table);
        marker = 40;
        src = srcBase + i * 13;
        do {
            dst[j] = *src++;
            j++;
        } while (j < 12);
        dst[12] = marker;
        i = next;
    } while (i < 4);
}
