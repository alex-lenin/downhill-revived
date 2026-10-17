/* UNIT 0x00126E08 0x00126F38 */
struct Vec4 {
    float x, y, z, w;
    Vec4(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
};

class Model {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual int v3();
};

struct ModelHolder {
    char pad0[0x8];
    void *x8;
};

struct Obj {
    char pad0[0x4];
    int active;                 /* 0x4 */
    char pad8[0x60];
    char x68[0xC8];
    Model *model;               /* 0x130 */
    void *x134;                 /* 0x134 */
    char pad138[0x10];
    int x148;
};

extern char D_0038DCE0[];
extern "C" {
void func_001AC880(void *p, float value);
void *func_00164A10(void *p);
void func_002182A8(void *p, int a);
void func_00192B38(Model *model, void *out);
void func_00164EE8(void *p, int a, int b, Vec4 *v, void *buf);
void func_00164FF0(void *p, int a, int b);
void func_00165CE0(void *p, int a);
}

extern "C" void func_00126E08(Obj *self, int a1, int a2)
{
    char buf[0x30];
    void *p;

    if (self->active == 0) {
        self->x148 = a1;
        func_001AC880(self->x68, 0.0f);
        if (self->x134 == 0) {
            p = func_00164A10(D_0038DCE0);
            self->x134 = p;
            if (p == 0) {
                return;
            }
        }
        self->active = 1;
        if (((ModelHolder *)self->model)->x8 != 0) {
            func_002182A8(((ModelHolder *)self->model)->x8, 0);
        }
        func_00192B38(self->model, buf);
        Vec4 vec(2.0f, 6.0f, 0.0f, 0.0f);
        func_00164EE8(self->x134, 0, self->model->v3(), &vec, buf);
        func_00164FF0(self->x134, a1, a2);
    } else if (self->x134 != 0) {
        func_00165CE0(self->x134, a1);
    }
}
