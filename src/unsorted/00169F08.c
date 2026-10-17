/* UNIT 0x00169F08 0x00169F48 */
struct Inner {
    char pad0[0x4098];
    unsigned long x4098;
};

struct Mid {
    char pad0[0x158];
    struct Inner *x158;
};

struct Obj {
    int x0;
    struct Mid *x4;
};

extern void func_00169F48(struct Obj *obj);

void func_00169F08(struct Obj *obj)
{
    if ((unsigned char)(obj->x4->x158->x4098 >> 2) & 1) {
        func_00169F48(obj);
    }
}
