/* UNIT 0x001B1068 0x001B1098 */
typedef struct Obj {
    char pad0[0x440];
    int x440;
} Obj;

extern void func_001B1100(Obj *obj, int a);
extern void func_001B1098(Obj *obj);

void func_001B1068(Obj *obj)
{
    func_001B1100(obj, obj->x440);
    func_001B1098(obj);
}
