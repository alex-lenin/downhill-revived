/* UNIT 0x00181C30 0x00181C78 */
typedef struct Obj { int x0; void *x4; int x8; int xC; int x10; int x14; } Obj;
extern char D_00285D28[];
extern void func_00181C98(Obj *);
Obj *func_00181C30(Obj *obj) {
    obj->x0 = 0;
    obj->x4 = D_00285D28;
    obj->x10 = 0x78;
    obj->x14 = 0x78;
    func_00181C98(obj);
    return obj;
}

