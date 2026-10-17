/* UNIT 0x00235420 0x00235460 */
typedef struct Obj { char pad0[0x5FC]; void *x5FC; int x600, x604, x608; } Obj;
extern char D_00288818[];
extern void func_00232F80(Obj *);
Obj *func_00235420(Obj *obj) {
    func_00232F80(obj);
    obj->x5FC = D_00288818;
    obj->x604 = 0;
    obj->x608 = 0;
    obj->x600 = 0;
    return obj;
}
