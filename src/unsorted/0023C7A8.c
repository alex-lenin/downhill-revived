/* UNIT 0x0023C7A8 0x0023C8D8 */
extern void *memcpy(void *, const void *, unsigned int);

/* Copies two source pieces (src1/n1, src2/n2) into two destination pieces (dst1/len1, dst2/len2). */
int func_0023C7A8(char *dst1, int len1, char *dst2, int len2, char *src1, int n1, char *src2, int n2)
{
    int rest;

    if (len1 + len2 < n1 + n2) {
        return 0;
    }
    if (n1 >= len1) {
        memcpy(dst1, src1, len1);
        memcpy(dst2, src1 + len1, n1 - len1);
        memcpy(dst2 + n1 - len1, src2, n2);
    } else {
        rest = len1 - n1;
        if (n2 >= rest) {
            memcpy(dst1, src1, n1);
            memcpy(dst1 + n1, src2, rest);
            memcpy(dst2, src2 + len1 - n1, n2 - rest);
        } else {
            memcpy(dst1, src1, n1);
            memcpy(dst1 + n1, src2, n2);
        }
    }
    return n1 + n2;
}
