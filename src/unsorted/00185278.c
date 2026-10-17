/* UNIT 0x00185278 0x001852A8 */
extern int strncmp(const char *, const char *, unsigned int);
int func_00185278(char *obj, const char *text, int index, unsigned int count) {
    return strncmp(text, obj + ((index << 5) + 8), count);
}
