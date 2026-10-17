/* UNIT 0x0018EE80 0x0018EEC0 */
typedef struct Obj {
    char pad0[0x4B0];
    void *x4B0;
} Obj;

extern char D_003F52D0[];
extern void func_0018E468(Obj *obj, int flag);
extern void func_001904B8(void *list, void *item);

void func_0018EE80(Obj *obj, int flag)
{
    if (flag) {
        func_0018E468(obj, flag);
    } else if (obj->x4B0 != 0) {
        func_001904B8(D_003F52D0, obj->x4B0);
    }
}
