/* UNIT 0x00157750 0x001577B0 */
extern void func_0012FE38(void *, void *);
extern void func_00133CB0(void *);
extern void func_001A5460(void *);
void func_00157750(void **obj) {
    char *p = *obj;
    if ((unsigned char)((unsigned long)*(long *)(p + 0x4098) >> 3) & 1) {
        func_0012FE38(p, p + 0x4ED0);
    } else {
        func_00133CB0(p + 0x6420);
        func_001A5460((char *)obj + 0x10);
    }
}
