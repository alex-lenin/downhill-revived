/* UNIT 0x0019BBA8 0x0019BBE0 */
void func_0019BBA8(void *p) {
    int count;
    int value;
    char *bytes;
    *(int *)p = 0;
    value = -1;
    bytes = (char *)p + 15;
    count = 11;
    do { *bytes = value; count--; bytes--; } while (count >= 0);
}
