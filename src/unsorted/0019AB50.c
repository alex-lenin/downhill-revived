/* UNIT 0x0019AB50 0x0019AB60 */
typedef struct {
    char pad0[0x504];
    int x504;
    int x508;
} Obj;

void func_0019AB50(Obj *obj, int value)
{
    obj->x504 = value;
    obj->x508 = 0xB4;
}
