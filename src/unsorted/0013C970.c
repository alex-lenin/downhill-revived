/* UNIT 0x0013C970 0x0013C9C8 */
struct Obj { char pad0[0x524]; int x524; int x528; };
extern char D_00362490[], D_0036B730[], D_003F52D0[];
extern void func_0013C400(struct Obj *, int);
extern void func_00140C38(void *);
extern void func_00190660(void *, void *);
void func_0013C970(struct Obj *obj) {
    if (obj->x528) {
        func_0013C400(obj, 0);
    } else if (!obj->x524) {
        func_00190660(D_003F52D0, D_00362490);
        func_00140C38(D_0036B730);
    }
}
