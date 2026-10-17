/* UNIT 0x0017F560 0x0017F5A8 */
extern void func_0017FDB8(void *);
void func_0017F560(char *obj) {
    char *item = obj + 0x30;
    int count = 39;
    do { --count; func_0017FDB8(item); item += 0x220; } while (count >= 0);
}
