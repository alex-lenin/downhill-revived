/* UNIT 0x00190890 0x001908D0 .rodata=0x28D7F0:0x28D830 */
typedef struct Obj {
    char pad0[0x150];
    int x150;
    char pad154[0x38];
    int x18C;
} Obj;

extern void func_00254D08(const char *fmt, ...);

void func_00190890(Obj *obj)
{
    if (obj->x18C != 0) {
        func_00254D08("ack! -overwriting return menu... this ain't good\n");
    }
    obj->x18C = obj->x150;
}
