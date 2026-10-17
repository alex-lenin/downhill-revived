/* UNIT 0x00141CA8 0x00141CD8 */
struct Obj {
    char pad0[0x50];
    int x50;
    int x54;
    char pad58[0x98];
    void *xF0;
};
extern void func_002182A8(void *, int);
void func_00141CA8(struct Obj *arg0) {
    arg0->x50 = 0;
    arg0->x54 = 0;
    if (arg0->xF0 != 0) {
        func_002182A8(arg0->xF0, 0);
    }
}
