/* UNIT 0x0019B740 0x0019B780 */
/* C++ constructor: base ctor, then x4F8 = vtable (D_...), x45C = 16. */
extern char D_00287988[];

class func_0018F210 {
public:
    func_0018F210();
    char pad0[0x45C];
};

class func_0019B740 : public func_0018F210 {
public:
    func_0019B740();
    int x45C;
    char pad460[0x98];
    void *x4F8;
};

func_0019B740::func_0019B740() {
    x4F8 = D_00287988;
    x45C = 0x10;
}
