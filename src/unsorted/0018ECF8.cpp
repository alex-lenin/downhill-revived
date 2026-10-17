/* UNIT 0x0018ECF8 0x0018ED70 */
class Node {
public:
    char pad0[0x4F8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
};
struct Obj { char pad0[0x500]; Node *x500[12]; int x530; };
void func_0018ECF8(Obj *obj) {
    int i;
    for (i = 0; i < obj->x530; i++) obj->x500[i]->v2();
}
