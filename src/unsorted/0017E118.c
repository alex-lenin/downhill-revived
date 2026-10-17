/* UNIT 0x0017E118 0x0017E1C0 */
struct Obj {
    char pad0[0x194];
    int x194[16];
    char pad1D4[0x25C - 0x1D4];
    int x25C[16];
    char pad29C[0x324 - 0x29C];
    int x324[16];
    char pad364[0x388 - 0x364];
    int x388[16];
    char pad3C8[0x3F0 - 0x3C8];
    unsigned char x3F0[16];
    char pad400[0x436 - 0x400];
    unsigned char x436[16];
};

extern void func_0017EB00(struct Obj *obj, int i, int value);

static inline int GetTimer(struct Obj *obj, int i) {
    if (obj->x3F0[i]) {
        return obj->x25C[i];
    } else {
        return obj->x194[i];
    }
}

void func_0017E118(struct Obj *obj, int value) {
    func_0017EB00(obj, 6, value);
    if (obj->x388[6] != 0) {
        return;
    }
    if (obj->x436[6] != 0) {
        if (GetTimer(obj, 6) != 0) {
            if (obj->x3F0[6]) {
                obj->x25C[6] = 0;
            } else {
                obj->x194[6] = 0;
                obj->x25C[6] = 0;
            }
        } else {
            obj->x436[6] = 0;
        }
    } else {
        if (GetTimer(obj, 6) != 0 && obj->x324[6] > 120000) {
            obj->x436[6] = 1;
        }
    }
}
