/* UNIT 0x001900C8 0x00190108 */
typedef struct Obj {
    char pad0[0x14C];
    int x14C;
    int x150;
} Obj;

extern void func_00190378(Obj *obj);
extern void func_0018D638(void);
extern int func_00238F48(void);

void func_001900C8(Obj *obj)
{
    obj->x150 = 0;
    func_00190378(obj);
    func_0018D638();
    obj->x14C = func_00238F48();
}
