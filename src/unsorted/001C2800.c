/* UNIT 0x001C2800 0x001C2848 */
extern char *func_00209108(void);
extern void func_00218278(char *a, int b);

struct Obj {
    char pad0[0xFC];
    char *xFC;
};

void func_001C2800(struct Obj *obj, int arg)
{
    obj->xFC = func_00209108();
    obj->xFC[4] = 0;
    func_00218278(obj->xFC, arg);
}
