/* UNIT 0x0013FC08 0x0013FC98 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Entry { char pad0[0xC]; short xC; char padE[2]; };

class ScreenBase {
public:
    char pad0[0x10];
    Entry x10[(0x4F8 - 0x10) / 0x10];
    char pad4F0[0x8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
};

class Screen : public ScreenBase {
public:
    char pad4FC[4];
    char x500;
    void func_0013FC08(int index);
};

extern char D_004B2A90[];
extern char D_003F52D0[];
extern "C" void func_001C57A8(void *, u128);
extern "C" void func_00190890(void *);
extern "C" void func_00187AB0(void);

void Screen::func_0013FC08(int index) {
    bool isBack = ((Entry *)(index * sizeof(Entry) + (char *)this + 0x10))->xC == 0x1A;
    if (isBack) {
        func_001C57A8(D_004B2A90, (u128)0x4B4341425F53L);
        v7();
    } else {
        x500 = 1;
        func_00190890(D_003F52D0);
        func_00187AB0();
    }
}
