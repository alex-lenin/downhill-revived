/* UNIT 0x0013FAE0 0x0013FC08 */
#include "menu_items.h"
class ScreenBase {
public:
    char pad0[0x418];
    int x418;
    char pad41C[0x474 - 0x41C];
    int x474;
    char pad478[0x4A8 - 0x478];
    int x4A8;
    char pad4AC[0x4F8 - 0x4AC];
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
    unsigned char x500;
    unsigned char x501;
    void func_0013FAE0();
};

extern unsigned char D_003F53FC[];
extern int D_0029DD30;
extern long D_0029DE68;
extern "C" void func_0018E638(void *, int, void *);
extern "C" void func_0018E6B8(void *, int, void *);
extern "C" void func_0018DC20(void *);

void Screen::func_0013FAE0()
{
    if (D_003F53FC[0] != 0) {
        x500 = 0;
    }
    if (x500 != 0) {
        v7();
        return;
    }
    x418 = 0;
    x474 = 1;
    func_0018E638(this, 0x40, &D_0029DD30);
    func_0018E3A8((struct SharedMenu *)(void *)(this), x501, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(this), 0, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x41, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x70, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x19, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1A, &D_0029DD30, 1);
    func_0018E6B8(this, 0x45, &D_0029DE68);
    x4A8 = 5;
    func_0018DC20(this);
}
