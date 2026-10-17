/* UNIT 0x0017D4C8 0x0017D520 */
extern void *func_0017D520(unsigned int);
extern void func_0017D580(void *, unsigned int *);
void *func_0017D4C8(unsigned int *obj) {
    void *value, *result;
    value = func_0017D520(*obj >> 18);
    if (value != 0) {
        func_0017D580(value, obj);
        result = value;
    } else {
        result = 0;
    }
    return result;
}
