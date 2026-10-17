/* UNIT 0x0015FAF0 0x0015FB58 */
struct Entry {
    char pad0[0xC];
    short xC;
    char padE[0x2];
    bool Is(int type) { return xC == type; }
};
struct Obj {
    char pad0[0x10];
    Entry entries[1];
    void func_0015C920();
    void func_0015FAF0(int index);
};
extern char D_003F52D0[], D_00388490[], D_003F53FD[];
extern "C" void func_00190660(void *, void *);
void Obj::func_0015FAF0(int index) {
    if (entries[index].Is(0x1A)) {
        func_00190660(D_003F52D0, D_00388490);
    } else if (entries[index].Is(0x22)) {
        func_0015C920();
        D_003F53FD[0] = 1;
    }
}
