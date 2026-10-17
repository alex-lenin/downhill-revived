/* UNIT 0x00135690 0x00135708 .rodata=0x2890A0:0x2890C0 */
struct Obj {
    char pad0[0x4AF];
    unsigned char state;
};

extern void func_00135990(struct Obj *obj);
extern void func_001359F8(struct Obj *obj);
extern void func_00135958(struct Obj *obj);
extern void func_001357D8(struct Obj *obj);

void func_00135690(struct Obj *obj)
{
    switch (obj->state) {
    case 0:
        break;
    case 1:
        func_00135990(obj);
        break;
    case 2:
        func_001359F8(obj);
        break;
    case 3:
        func_00135958(obj);
        break;
    case 4:
        func_001357D8(obj);
        break;
    }
}
