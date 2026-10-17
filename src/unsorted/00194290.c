/* UNIT 0x00194290 0x00194430 */
struct Triple { int x0, x4, x8; };
extern char D_004C8508[];
extern struct Triple D_0028D890, D_0028D8A0, D_0028D8B0, D_0028D8C0, D_0028D8D0, D_0028D8E0;
extern void func_001F9C78(void *, struct Triple *);

void func_00194290(char *obj, int index) {
    struct Triple a, b;
    int old = *(int *)(obj + 0x12110);
    int mask = 1 << index;

    *(int *)(obj + 0x12110) = old | mask;
    if (!(old & mask)) {
        void *p = D_004C8508;
        /* ids held in locals: cc1 keeps the li loads ahead of the compare chain */
        int k6 = 6, k7 = 7, k9 = 9, k11 = 11, k12 = 12;
        if (index == k6) {
            a = D_0028D890;
            func_001F9C78(p, &a);
            b = D_0028D8A0;
            func_001F9C78(p, &b);
        } else if (index == k7) {
            a = D_0028D8B0;
            func_001F9C78(p, &a);
        } else if (index == k9) {
            a = D_0028D8C0;
            func_001F9C78(p, &a);
        } else if (index == k11) {
            a = D_0028D8D0;
            func_001F9C78(p, &a);
        } else if (index == k12) {
            a = D_0028D8E0;
            func_001F9C78(p, &a);
        }
    }
}
