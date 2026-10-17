/* UNIT 0x001E2118 0x001E2168 */
extern void func_001E2D78(void *);
void func_001E2118(void *obj, int value) {
    int i;
    int *p;
    i = 3;
    p = (int *)((char *)obj + 0x510);
    for (; i >= 0; i--, p--) *p = 0;
    *(int *)((char *)obj + 0x51c) = value;
    *(int *)((char *)obj + 0x514) = 0;
    func_001E2D78(obj);
}
