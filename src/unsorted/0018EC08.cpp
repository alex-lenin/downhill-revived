/* UNIT 0x0018EC08 0x0018ECF8 */
class Node {
public:
    char pad0[0x4B9];
    unsigned char x4B9;
    char pad4BA[0x4F8 - 0x4BA];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
};

struct Obj {
    char pad0[0x46C];
    int x46C;
    char pad470[0x478 - 0x470];
    int x478;
    char pad47C[0x4A8 - 0x47C];
    int x4A8;
    char pad4AC[0x500 - 0x4AC];
    Node *x500[12];
    int x530;
};

extern void func_0018DBA8(Obj *obj);

void func_0018EC08(Obj *obj)
{
    int i;

    obj->x4A8 = 5;
    obj->x478 = 1;
    obj->x46C = 0;
    for (i = 0; i < obj->x530; i++) {
        if (obj->x500[i]->x4B9) {
            obj->x500[i]->v0();
            obj->x500[i]->x4B9 = 1;
        }
    }
    for (i = 0; i < obj->x530; i++) {
        obj->x500[i]->v4();
    }
    func_0018DBA8(obj);
}
