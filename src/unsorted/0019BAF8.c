/* UNIT 0x0019BAF8 0x0019BB30 */
struct Unk398C40 {
    char pad0[0x8];
    char x8[1];
};

extern struct Unk398C40 D_00398C40;
extern void func_001DE8D0(void *);
extern void func_0018E468(void *);

static __inline__ struct Unk398C40 *GetUnk(void) {
    return &D_00398C40;
}

void func_0019BAF8(void *obj) {
    func_001DE8D0(GetUnk()->x8);
    func_0018E468(obj);
}
