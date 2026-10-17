/* UNIT 0x00134AC0 0x00134AF8 */
typedef struct Obj { char pad0[0x20]; void *x20; } Obj;
extern void func_00134AA8(Obj *);
extern char D_00283B20[];
Obj *func_00134AC0(Obj *obj) {
    func_00134AA8(obj);
    obj->x20 = D_00283B20;
    return obj;
}
