/* UNIT 0x00174790 0x001747C8 */
typedef struct {
    char pad0[0x8];
    char x8;
    char pad9[0x28 - 0x9];
    char x28;
    char x29;
    char x2A;
    char x2B;
} Obj;

extern void func_001747C8(Obj *obj);

void func_00174790(Obj *obj)
{
    func_001747C8(obj);
    obj->x8 = 0;
    obj->x28 = 0;
    obj->x29 = 0;
    obj->x2A = 0;
    obj->x2B = 0;
}
