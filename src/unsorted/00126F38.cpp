/* UNIT 0x00126F38 0x00127098 */
#include "model.h"
struct Vec4 {
    float x, y, z, w;
    Vec4(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
};

struct ModelInfo {
    long flags;
};

class Model {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual ModelInfo *v3();
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
};

extern char D_0038DCE0[];
extern "C" {


void func_001AC880(void *p, float value);
void *func_00164A10(void *p);
void func_002182A8(void *p, int a);
void func_00192B38(Model *model, void *out);
void func_00164EE8(void *p, int a, ModelInfo *b, Vec4 *v, void *buf);
void func_00165408(void *p, int a, int b);
}

extern "C" void func_00126F38(Obj *self, int a1, int a2)
{
    char buf[0x30];
    void *p;

    if ((self->model->v3()->flags & 0x200000000000000L) == 0
        && func_00192BA8((int)(func_002623C0(self->model))) == 10) {
        return;
    }
    if (self->active == 0) {
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
        func_00165408(self->x134, a2, a1);
    }
}
