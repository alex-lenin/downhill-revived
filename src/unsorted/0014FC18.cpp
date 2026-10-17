/* UNIT 0x0014FC18 0x0014FC50 */
struct Obj {
    int index;
    void func_0014FC50();
    void func_0014FC18();
};
void Obj::func_0014FC18() {
    int value = *(short *)(index * 16 + (char *)this + 0x1C);
    if ((value ^ 0x24) == 0) {
        func_0014FC50();
    }
}
