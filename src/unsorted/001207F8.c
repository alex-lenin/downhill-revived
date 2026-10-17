/* UNIT 0x001207F8 0x00120838 */
typedef struct Obj { char pad0[0x262]; short x262; } Obj;
extern void func_0011FAA0(Obj *);
extern void func_0011FE08(Obj *);
void func_001207F8(Obj *obj) {
    if (--obj->x262) func_0011FAA0(obj);
    else func_0011FE08(obj);
}

