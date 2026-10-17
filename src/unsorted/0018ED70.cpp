/* UNIT 0x0018ED70 0x0018EE18 */
class Child {
public:
 char pad[0x4f8];
 virtual void v1();
 virtual void v2();
 virtual void v3();
 virtual void v4(int, int, int);
};
struct Obj { char pad[0x500]; Child *children[12]; int count; };
void func_0018ED70(Obj *obj, int a, int b, int c) {
 int i;
 for (i = 0; i < obj->count; i++) obj->children[i]->v4(a, b, c);
}
