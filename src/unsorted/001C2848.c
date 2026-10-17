/* UNIT 0x001C2848 0x001C2880 */
typedef struct Obj { char pad0[0xFC]; void *xFC; } Obj;
extern void func_002091A0(void *);
void func_001C2848(Obj *obj) {
    if (obj->xFC != 0) {
        func_002091A0(obj->xFC);
        obj->xFC = 0;
    }
}
