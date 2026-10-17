/* UNIT 0x0018DBA8 0x0018DC20 */
struct Obj;
class Node {
public:
    virtual void v0(Obj *);
};
struct Obj { char pad0[0x4CC]; Node *x4CC[10]; int x4F4; };
void func_0018DBA8(Obj *obj) {
    int i;
    for (i = 0; i < obj->x4F4; i++) obj->x4CC[i]->v0(obj);
}
