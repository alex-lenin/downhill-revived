/* UNIT 0x0018D688 0x0018D6F0 */
struct Entry {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
};
extern struct Entry D_003F4BA0[];
extern void func_0018D7B0(struct Entry *);

void func_0018D688(void) {
    int i;

    for (i = 0; i < 20; i++) {
        if (D_003F4BA0[i].x8 != 0) {
            func_0018D7B0(&D_003F4BA0[i]);
        }
    }
}
