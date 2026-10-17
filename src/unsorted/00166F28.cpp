/* UNIT 0x00166F28 0x00166F68 */
extern char D_002857C0[];

class func_0018D9D0 {
public:
    func_0018D9D0(void);
    char pad0[0x45C];
};

class func_00166F28 : public func_0018D9D0 {
public:
    func_00166F28();
    int x45C;
    char pad460[0x98];
    void *x4F8;
};

func_00166F28::func_00166F28() {
    x4F8 = D_002857C0;
    x45C = 0x10;
}
