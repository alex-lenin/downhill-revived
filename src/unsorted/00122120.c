/* UNIT 0x00122120 0x00122188 */
extern char D_002FF6A0[];
extern void func_00260FD8(void *);
void func_00122120(int active, int phase) {
    int count;
    char *obj;
    if (phase == 0xFFFF && active) {
        count = 29;
        obj = D_002FF6A0;
        do {
            count--;
            func_00260FD8(obj);
            obj += 0x270;
        } while (count != -1);
    }
}
