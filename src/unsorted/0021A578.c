/* UNIT 0x0021A578 0x0021A5C8 */
typedef struct { unsigned int count; char pad[6]; unsigned char xa, xb; } List;
int func_0021A578(unsigned char a, unsigned char b, List *list) {
    unsigned int i;
    for (i = 0; i < list->count; i++) {
        unsigned char *p = (unsigned char *)list + i * 8;
        if (p[10] == a && p[11] == b) return i;
    }
    return -1;
}
