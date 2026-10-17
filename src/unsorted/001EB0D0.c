/* UNIT 0x001EB0D0 0x001EB0F8 */
typedef struct {
    char pad0[0x8];
    char x8;
} Obj;

extern void func_001EB0F8(Obj *obj);

void func_001EB0D0(Obj *obj)
{
    func_001EB0F8(obj);
    obj->x8 = 0;
}
