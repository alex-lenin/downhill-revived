/* UNIT 0x001DCD38 0x001DCD60 */
struct Obj {
    char pad0[0x30];
    void *x30;
};

extern void func_00211280(void *p);

void func_001DCD38(struct Obj *obj) {
    if (obj->x30 != 0) {
        func_00211280(obj->x30);
    }
}
