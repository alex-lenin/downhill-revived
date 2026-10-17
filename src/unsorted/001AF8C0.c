/* UNIT 0x001AF8C0 0x001AF8D0 */
typedef struct Obj {
    int x0;
    char pad4[0x6C];
    int x70;
} Obj;

void func_001AF8C0(Obj *obj)
{
    obj->x70 = 0;
    obj->x0 = 0;
}
