/* UNIT 0x00167B88 0x00167BC0 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
} Obj;
extern void func_00220550(int value);
extern void func_00190688(void *obj);
extern char D_003F52D0[];

void func_00167B88(Obj *obj) {
    if (obj->x500 != 0) {
        func_00220550(1);
    }
    func_00190688(D_003F52D0);
}
