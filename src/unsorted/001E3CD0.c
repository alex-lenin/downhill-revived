/* UNIT 0x001E3CD0 0x001E3DC0 */
#include "menu_items.h"
/* The retail function is entered two words into its range: the code pointer
   in the table at 0x002882CC is 0x001E3CD8 and nothing references 0x001E3CD0.
   The first two words (`sw $zero,0x500($a0)' / `nop', 0xAC800500) are never
   executed and cc1 does not emit them from C, so they are provided here as
   data words in .text, with the C body emitted immediately after them. */
__attribute__((section(".text"))) static const unsigned int prefunc[2] = { 0xAC800500, 0x00000000 };

struct Obj {
    char pad0[0x418];
    int x418;
    char pad41c[0x44];
    int x460;
    char pad464[0x9c];
    int x500;
    char pad504[0x14];
    unsigned char x518;
};

extern struct Obj D_004C5640;
extern char D_003F52D0[];
extern long D_0029DE68;
extern void func_001B5AF0(void *, int, int);
extern void func_0018E6B8(void *, int, void *);
extern void func_0018DB08(void *);

void func_001E3CD0_body(struct Obj *obj)
{
    obj->x418 = 0;
    func_001B5AF0(D_003F52D0, 1, 1);
    if (D_004C5640.x518) {
        if (D_004C5640.x500 + 1 != 15)
            func_0018E3A8((struct SharedMenu *)(void *)(obj), 27, &D_0029DE68, 1);
    } else {
        func_0018E3A8((struct SharedMenu *)(void *)(obj), 40, &D_0029DE68, 1);
    }
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 152, &D_0029DE68, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 26, &D_0029DE68, 1);
    func_0018E6B8(obj, 168, &D_0029DE68);
    obj->x460 = 155;
    func_0018DB08(obj);
}
