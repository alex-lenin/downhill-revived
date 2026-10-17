/* UNIT 0x0018CFC8 0x0018D028 */
struct Child { char pad[4]; unsigned char x4; };
struct Obj { char pad[4]; struct Child *x4; };
extern int D_0029E4C4;
extern int func_00238F48(void);
extern void func_0018D028(struct Obj *);
void func_0018CFC8(struct Obj *obj) {
    struct Child *child;
    if (D_0029E4C4 != func_00238F48()) {
        func_0018D028(obj);
        D_0029E4C4 = func_00238F48();
    }
    child = obj->x4;
    if (child) child->x4 = 1;
}
