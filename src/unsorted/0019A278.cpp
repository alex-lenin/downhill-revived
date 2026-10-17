/* UNIT 0x0019A278 0x0019A2B8 */
/* C++ constructor: base ctor, then x4F8 = vtable (D_...), x45C = 16. */
extern char D_00287B20[];

class func_0018D9D0 {
public:
    func_0018D9D0(void);
    char pad0[0x45C];
};

class func_0019A278 : public func_0018D9D0 {
public:
    func_0019A278();
    int x45C;
    char pad460[0x98];
    void *x4F8;
};

func_0019A278::func_0019A278() {
    x4F8 = D_00287B20;
    x45C = 0x10;
}
