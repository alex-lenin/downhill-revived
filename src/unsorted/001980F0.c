/* UNIT 0x001980F0 0x00198140 */
typedef struct Obj { char pad[0x500]; int x500, x504; } Obj;
extern Obj D_00435F20;
extern char D_003F52D0[];
extern void func_00190660(void *, Obj *);
void func_001980F0(Obj *p) {
    D_00435F20.x500 = 1;
    func_00190660(D_003F52D0, &D_00435F20);
    D_00435F20.x504 = p->x500;
}
