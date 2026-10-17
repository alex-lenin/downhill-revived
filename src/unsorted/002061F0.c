/* UNIT 0x002061F0 0x00206258 */
typedef struct Target { char pad[0x18]; int x18; int x1C; int x20; float x24, x28; } Target;
typedef struct Other { char pad[8]; int x8; } Other;
typedef struct Context { char pad[0x24]; Other *x24; } Context;
typedef struct Obj { int x0; char x4; char pad[0xF]; Target *x14; int x18; int x1C; char pad20[0x10]; int x30; float x34, x38; } Obj;
extern void func_00206258(int, Context *);
void func_002061F0(Obj *obj, Context *context) {
    obj->x4 = 0;
    if (obj->x14) {
        obj->x14->x18 = obj->x1C;
        obj->x14->x20 = obj->x30;
        obj->x14->x24 = obj->x34;
        obj->x14->x28 = obj->x38;
    } else {
        context->x24->x8 = obj->x1C;
    }
    func_00206258(obj->x18, context);
}
