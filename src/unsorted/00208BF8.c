/* UNIT 0x00208BF8 0x00208C50 */
extern void func_00201C58(int);
extern void func_00208AE0(void *, int *);
void func_00208BF8(void *obj, int *value) {
    int current = *value;
    if (current != 0xFFFFFFFF) {
        if (current != 0) {
            func_00201C58(current);
            *value = 0;
        }
    } else {
        func_00208AE0(obj, value);
    }
}
