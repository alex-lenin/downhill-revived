/* UNIT 0x0014ED28 0x0014EFA0 */
#include "menu_items.h"
struct State {
    char pad0[0x10C];
    int mode;
};

/* 0x18-byte entries indexed by State::mode; the tests are inline methods
   (written as plain field reads, gcc CSEs the second index multiply). */
struct ModeInfo {
    char pad0[8];
    int x8;
    int xC;
    char pad10[8];
    int HasX8() { return x8 != 0; }
    int HasXC() { return xC != 0; }
};

extern State D_004B25A0;
extern ModeInfo D_00381CF0[];
extern int D_0029DE68;
extern unsigned char D_0043DD41[];
extern char D_0043F870[];

extern void func_001A22D8(void *p);
extern void func_0018DC20(void *menu);

struct Menu {
    int index;
    char pad4[0x418 - 4];
    int count;
    char pad41C[0x460 - 0x41C];
    int x460;
    void func_0014ED28();
    void func_0014FC18();
};

void Menu::func_0014ED28()
{
    short value;

    value = *(short *)(index * 16 + (char *)this + 0x1C);
    if ((value ^ 0x24) == 0) {
        func_0014FC18();
    } else {
        func_001A22D8(D_0043F870);
        x460 = 0xA5;
        count = 0;
        if (D_00381CF0[D_004B25A0.mode].HasX8()) {
            if (!D_00381CF0[D_004B25A0.mode].HasXC()) {
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1F, &D_0029DE68, 1);
                if (D_004B25A0.mode != 4) {
                    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x24, &D_0029DE68, 1);
                }
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x30, &D_0029DE68, 1);
                if (D_0043DD41[0] == 0) {
                    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1D, &D_0029DE68, 1);
                }
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x23, &D_0029DE68, 1);
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1A, &D_0029DE68, 1);
            } else {
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x30, &D_0029DE68, 1);
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x3A, &D_0029DE68, 1);
                if (D_0043DD41[0] == 0) {
                    func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1D, &D_0029DE68, 1);
                }
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x23, &D_0029DE68, 1);
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x9A, &D_0029DE68, 1);
            }
        } else {
            func_0018E3A8((struct SharedMenu *)(void *)(this), 0x20, &D_0029DE68, 1);
            if (D_004B25A0.mode != 4) {
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x24, &D_0029DE68, 1);
            }
            func_0018E3A8((struct SharedMenu *)(void *)(this), 0x30, &D_0029DE68, 1);
            if (D_0043DD41[0] == 0) {
                func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1D, &D_0029DE68, 1);
            }
            func_0018E3A8((struct SharedMenu *)(void *)(this), 0x23, &D_0029DE68, 1);
            func_0018E3A8((struct SharedMenu *)(void *)(this), 0x1A, &D_0029DE68, 1);
        }
    }
    func_0018DC20(this);
}
