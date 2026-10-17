/* UNIT 0x00190358 0x00190378 */
typedef struct Obj {
    char pad0[0x64];
    char x64[1];
} Obj;

extern void func_001909A8(void *p);

void func_00190358(Obj *obj)
{
    func_001909A8(obj->x64);
}
