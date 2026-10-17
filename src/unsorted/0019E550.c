/* UNIT 0x0019E550 0x0019E7E8 */
#include "menu_items.h"
struct Obj {char pad0[0x618]; int x618; char x61C[9]; unsigned char x625;};
struct Small {int x0; char pad4[0x60C];};
struct Large {char pad0[0xC94]; unsigned char xC94; char padC95[0xB];};
extern void *func_00184940(void *);
extern void func_00184A78(void *);
extern void func_00184AA0(void *,char *,void *,int);
extern void func_001852A8(void *,void *);
extern int func_00185938(void *,void *);
extern void func_00187048(void *,void *);
extern void func_001870E0(void *,void *);
extern void func_001873F8(void *);
extern void func_0018DB20(void *);
extern int func_0019C210(void *);
extern void func_0019CB08(void *,int,void *);
extern void func_00218A30(void);
extern int func_0021F818(void);
extern int func_00254D68(void *,void *,...);
extern char D_0028E5D0[],D_0028E5E0[],D_0029D1C8[],D_003E3BD0[],D_003EB760[],D_003EB781[],D_0043DD10[],D_0043DD38[],D_00528980[];
void func_0019E550(struct Obj *arg0) {
    char name[0x50];
    int sp50[4];
    int sp60[4];
    struct Small sp70;
    struct Large sp680;
    int sp1320[8];
    char *sp1340;
    void *empty; struct Small *player; struct Large *large;
    int var_s0;
    int var_v1;
    char *var_v0;

    if (arg0->x625 != 0) {
        arg0->x625 = 0U;
        sp1340 = arg0->x61C;
        empty=&sp60;player=&sp70;large=&sp680;
        arg0->x618 = 0;
        var_v1 = 8;
        var_v0 = arg0->x61C + 8;
        do {
            *var_v0 = 0;
            var_v1 -= 1;
            var_v0 -= 1;
        } while (var_v1 >= 0);
        func_00218A30();
        func_00254D68(&D_0043DD10, &D_0029D1C8, &D_003EB781);
        func_00184940(empty);
        sp70.x0 = -1;
        func_0018DB20(player);
        func_001873F8(player);
        func_001873C0((struct SharedMenu *)(void *)(player), 0x3C, 0);
        func_001873C0((struct SharedMenu *)(void *)(player), 0xC, 0);
        func_001873C0((struct SharedMenu *)(void *)(player), func_0021F818() + 0x10, 0);
        func_001873C0((struct SharedMenu *)(void *)(player), 0, 0);
        func_001873C0((struct SharedMenu *)(void *)(player), 0xE, 0);
        func_001873C0((struct SharedMenu *)(void *)(player), 0xC, 0);
        func_001873C0((struct SharedMenu *)(void *)(player), 0xF, 0);
        func_001852A8(large, &D_0043DD10);
        func_00187048(&D_003E3BD0, empty);
        func_00187048(&D_003E3BD0, large);
        func_001870E0(&D_003E3BD0, D_003EB781 - 0x21);
        if (sp680.xC94 != 0) {
            var_s0 = 0;
            do {
                func_00254D68(name, &D_0028E5D0, &D_0043DD10, var_s0);
                func_00254D68(&sp50, &D_0028E5E0, var_s0);
                if (func_00185938(large, &sp50) != 0) {
                    func_00184A78(&sp1320);
                    func_00184AA0(&sp1320, name, &D_00528980, func_0019C210(&D_0043DD38));
                    func_00187048(&D_003E3BD0, &sp1320);
                    func_00187048(&D_003E3BD0, empty);
                    func_001870E0(&D_003E3BD0, &D_003EB760);
                    func_0019CB08(&D_0043DD38, var_s0, &D_00528980);
                    arg0->x618 = (int) (arg0->x618 + 1);
                    *(sp1340 + var_s0) = 1;
                }
                var_s0 += 1;
            } while ((int) var_s0 < 9);
        }
    }
}
