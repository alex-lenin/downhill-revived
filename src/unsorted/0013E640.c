/* UNIT 0x0013E640 0x0013E760 .rodata=0x289AA8:0x289AB8 */
#include "menu_items.h"
struct Res { char pad0[0x17C]; char *x17C; };
struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x474 - 0x41C];
    int x474;
    char pad478[0x500 - 0x478];
    char text[0xAC];
    int x5AC;
    int x5B0;
    int x5B4;
    char x5B8;
};

extern struct Res *D_0029DD30;
extern void func_0018E638(struct Obj *, int, struct Res **);
extern int func_001FA370(int, int);
extern void func_00254D68(char *, const char *, ...);
extern void func_0018E408(struct Obj *, char *, int);

void func_0013E640(struct Obj *obj, int a, int b, int count, int half)
{
    char *text;
    char *name;

    obj->x5AC = a;
    obj->x5B0 = b;
    obj->x5B4 = count;
    obj->x5B8 = half;
    if (half) obj->x5B4 = count / 2;
    obj->x418 = 0;
    obj->x474 = 1;
    func_0018E638(obj, half + 0x17, &D_0029DD30);
    text = obj->text;
    name = D_0029DD30->x17C;
    func_00254D68(text, "%s%d - %s", name, obj->x5B4, func_001FA370(obj->x5AC, b));
    func_0018E408(obj, text, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0, &D_0029DD30, 0);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x19, &D_0029DD30, 1);
    func_0018E3A8((struct SharedMenu *)(void *)(obj), 0x1A, &D_0029DD30, 1);
}
