/* UNIT 0x00184330 0x00184380 */
typedef struct Obj { int x0; void *x4; char pad[8]; int x10; int x14; int x18; int x1C; } Obj;
extern char D_00285F70[];
extern void func_001843A8(Obj *);
Obj *func_00184330(Obj *obj)
{
    obj->x0 = 0;
    obj->x10 = 0;
    obj->x4 = D_00285F70;
    obj->x18 = 400;
    obj->x1C = 400;
    obj->x14 = 5600;
    func_001843A8(obj);
    return obj;
}






