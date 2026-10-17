/* UNIT 0x00166E98 0x00166F00 */
struct Obj {
    char pad0[0x460];
};

extern struct Obj D_0038DCE0[];
extern struct Obj *func_00164C38(struct Obj *obj);

/* static initializer: constructs a global array of 30 objects */
void func_00166E98(int initialize, int priority) {
    struct Obj *obj;
    int i;

    if (priority == 0xFFFF && initialize != 0) {
        obj = D_0038DCE0;
        i = 29;
        do {
            func_00164C38(obj);
            obj++;
        } while (--i != -1);
    }
}
