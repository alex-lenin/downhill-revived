/* UNIT 0x00197640 0x00197688 */
/* func_00197640 is a C++ constructor (returns this; its register use only
   matches gcc's constructor codegen, including the base-constructor call).
   The classes are named after their constructors' functions so the mangled
   names (__13func_00197640, __13func_0018D9D0) link to the splat symbols.
   D_002877C0 is the class's vtable, stored at 0x4F8. */
extern char D_002877C0[];

struct func_0018D9D0 {
    char pad0[0x4F8];
    void *x4F8;
    func_0018D9D0(void);
};

struct func_00197640 : func_0018D9D0 {
    char pad4FC[0x500 - 0x4FC];
    int x500;
    int x504;
    int x508;
    func_00197640();
};

func_00197640::func_00197640()
{
    x4F8 = D_002877C0;
    x508 = -1;
    x500 = -1;
    x504 = -1;
}
