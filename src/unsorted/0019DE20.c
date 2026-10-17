/* UNIT 0x0019DE20 0x0019DE30 */
typedef struct {
    char pad0[0x504];
    int x504;
    char pad508[0x18];
    char x520;
} Obj;

void func_0019DE20(Obj *obj, char value)
{
    obj->x520 = value;
    obj->x504 = 4;
}
