/* UNIT 0x0015C240 0x0015C288 */
struct Obj {
    char *x0;
    int x4;
    int x8;
};

extern void func_00202E38(char *p, int a, int b, float f);
extern int func_00238F48(void);

void func_0015C240(struct Obj *obj)
{
    func_00202E38(obj->x0 + 0x20, 1, 1, 100.0f);
    obj->x8 = func_00238F48();
}
