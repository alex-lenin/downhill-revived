/* UNIT 0x00140C38 0x00140C68 */
struct Obj {
    char pad0[0x60];
    char x60[0x30];
    unsigned char x90;
    char pad91[0x9B];
    unsigned char x12C;
};
extern void func_00208118(void *);
void func_00140C38(struct Obj *arg0) {
    if (arg0->x12C != 0) {
        arg0->x90 = 1;
        func_00208118(arg0->x60);
    }
}
