/* UNIT 0x00191748 0x00191790 */
extern void func_001919E8(void *);
void func_00191748(char *obj) {
    char *item = obj + 0x10;
    int count = 2;
    do { --count; func_001919E8(item); item += 0xA0; } while (count >= 0);
}
