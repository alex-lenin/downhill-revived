/* UNIT 0x00210698 0x002106C8 */
typedef struct { char pad0[0x30]; int x30; } Obj;
extern void func_00210868(Obj *);
int func_00210698(Obj *obj) {
    func_00210868(obj);
    return obj->x30 != 0;
}
