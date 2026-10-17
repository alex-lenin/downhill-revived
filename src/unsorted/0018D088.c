/* UNIT 0x0018D088 0x0018D130 */
struct Obj {
    char pad0[0x500];
    unsigned char x500;
};
extern char D_003F52D0[];
extern char D_003EBFD0[];
extern int func_001F7A78(void);
extern void func_001F79D8(int, int);
extern void func_0018C6E0(void);
extern void func_001F67F0(int);
extern void func_00190660(void *, void *);
extern void func_00190528(void *, int, int);

void func_0018D088(struct Obj *obj) {
    if (obj->x500 != 0 && func_001F7A78() == 0x80) {
        func_001F79D8(0, 16);
        func_0018C6E0();
        func_001F67F0(1);
        func_00190660(D_003F52D0, D_003EBFD0);
        obj->x500 = 0;
    } else if (func_001F7A78() == 0x80) {
        func_00190528(D_003F52D0, 0, 0);
        func_001F79D8(0, 16);
    }
}
