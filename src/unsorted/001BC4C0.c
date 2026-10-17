/* UNIT 0x001BC4C0 0x001BC4F0 */
struct Obj001BC4C0 {
    char pad0[0x830];
    int x830;
    int x834;
};

extern int func_00238F48(void);

void func_001BC4C0(struct Obj001BC4C0 *obj)
{
    obj->x830 = func_00238F48();
    obj->x834 = 0;
}
