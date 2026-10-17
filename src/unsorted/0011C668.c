/* UNIT 0x0011C668 0x0011C6D0 */
extern char D_002AF2C0[];
extern void func_0012B620(void *);
void func_0011C668(int active, int phase) {
    int count;
    char *obj;
    if (phase == 0xFFFF && active) {
        count = 9;
        obj = D_002AF2C0;
        do {
            count--;
            func_0012B620(obj);
            obj += 0x7BD0;
        } while (count != -1);
    }
}
