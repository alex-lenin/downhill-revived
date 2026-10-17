/* UNIT 0x00187068 0x001870E0 */
class Node {
public:
    int x0;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
};
struct Obj { char pad0[0x64]; Node *x64[2]; int x6C; };
void func_00187068(Obj *obj) {
    int i;
    for (i = 0; i < obj->x6C; i++) obj->x64[i]->v4();
}
