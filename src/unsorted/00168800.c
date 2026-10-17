/* UNIT 0x00168800 0x001688C8 */
#include "menu_items.h"
typedef struct Obj {
 int unk0;
 char pad4[1044];
 int unk418;
 char pad41c[68];
 int unk460;
} Obj;
extern void func_001B5AF0(void *,int,int);
extern void func_0018DB08(void *);
extern char D_003F52D0[];
extern int D_0029DE68;
void func_00168800(Obj *arg0) {
    func_001B5AF0(&D_003F52D0, 1, 1);
    arg0->unk418 = 0;
    func_0018E3A8((struct SharedMenu *)(void *)(arg0), 0x28, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(arg0), 0x47, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(arg0), 0x29, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(arg0), 0x23, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(arg0), 0x1A, &D_0029DE68, 1);
    arg0->unk0 = 0;
    arg0->unk460 = 0xA5;
    func_0018DB08(arg0);
}
