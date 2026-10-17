/* UNIT 0x00197810 0x00197840 */
typedef struct { char pad0[0x500]; int x500; } Object;
extern Object D_003F52D0, D_00435A10;
extern void func_00190660(Object *, Object *);
void func_00197810(Object *arg0) {
    D_00435A10.x500 = arg0->x500;
    func_00190660(&D_003F52D0, &D_00435A10);
}
