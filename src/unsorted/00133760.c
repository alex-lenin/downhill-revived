/* UNIT 0x00133760 0x001337B8 */
struct Obj {
    void *vtbl;
    int x4;
    char pad8[8];
    char x10[0x1D0];
    unsigned char x1E0;
};

extern char D_00283B60[];
/* Constructor of the member at 0x10; like every g++ ctor it returns this, and the
   non-void return is what puts the vtable store in the jal delay slot. */
extern void *func_002318C0(void *p);
extern void func_001337B8(struct Obj *obj);

struct Obj *func_00133760(struct Obj *obj, int value)
{
    obj->vtbl = D_00283B60;
    func_002318C0(obj->x10);
    obj->x4 = value;
    obj->x1E0 = 0;
    func_001337B8(obj);
    return obj;
}
