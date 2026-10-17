/* UNIT 0x0015BDA0 0x0015BE88 */
struct Vec4 {
    float x, y, z, w;
    Vec4(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
};

struct EffectDesc {
    void *x0;
    void *x4;
    int x8;
    void *xC;
    char pad10[0x18];
};

class Base {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual int v3();
};

class Obj : public Base {
public:
    char pad4[0x20];
    void *x24;
    char pad28[0x80];
    char xA8[0x10];
    char xB8[0x10];
    char xC8[0x10];
};

extern char D_0038DCE0[];
extern "C" {
void *func_00164A10(void *p);
void func_00159CE0(Obj *obj, int state);
void func_00254204(void *p, int c, int n);
void func_00164EE8(void *p, int a, int b, Vec4 *v, void *buf);
void func_00164FF0(void *p, int a, int b);
}

extern "C" void func_0015BDA0(Obj *obj, int a1, int a2)
{
    EffectDesc desc;
    void *p;

    if (obj->x24 == 0) {
        p = func_00164A10(D_0038DCE0);
        obj->x24 = p;
        if (p != 0) {
            func_00159CE0(obj, 8);
            func_00254204(&desc, 0, sizeof(desc));
            desc.x0 = obj->xA8;
            desc.x4 = obj->xC8;
            desc.xC = obj->xB8;
            Vec4 vec(4.0f, 4.0f, 0.0f, 0.0f);
            func_00164EE8(obj->x24, 2, obj->v3(), &vec, &desc);
        }
    }
    func_00164FF0(obj->x24, a1, a2);
}
