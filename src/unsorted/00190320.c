/* UNIT 0x00190320 0x00190358 */
struct Obj {
    char pad0[0x130];
    int x130;
    char pad134[0x184 - 0x134];
    int x184;
};

extern void func_001B4CD0(struct Obj *);
extern void func_0018D688(void);

void func_00190320(struct Obj *obj)
{
    obj->x130 = 0;
    func_001B4CD0(obj);
    func_0018D688();
    obj->x184 = 0;
}
