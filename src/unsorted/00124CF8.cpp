/* UNIT 0x00124CF8 0x00125178 .rodata=0x288D80:0x288DD0 .sdata=0x29C1D8:0x29C1E8 */
#include "model.h"
struct Vec { float x, y, z, w; } __attribute__((aligned(16)));
struct Model { unsigned long flags; char pad8[0x28]; Vec x30; };
class Part { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual Model *GetModel(); };

class Obj {
public:
    char pad0[0x8];
    int x8;
    char padC[0x34];
    int x40;
    int x44;
    int x48;
    int x4C;
    int x50;
    int x54;
    char pad58[0x74];
    float xCC;
    char padD0[0x60];
    Part *x130;
    int x134;
    char pad138[0x14];
    int (Obj::*x14C)();
    int (Obj::*x154)();

    int func_00124CE8();
    int func_00125178();
    void func_00124CF8();
};

extern "C" {
int func_00222BD8(int, int);
float func_00222B68(float, float);


}
extern int D_004B2840[];

#define MODEL_ID(m) (**(unsigned int **)(m) & 0xFFFC0000)

static inline void SetScale(Model *model, float scale)
{
    Vec v;
    model->flags |= 0x0100000000000000;
    v.x = scale;
    v.y = scale;
    v.z = scale;
    model->x30 = v;
}

static inline int IsBig(Model *model)
{
    unsigned int id = MODEL_ID(model);
    return id == 0x3FAC0000 || id == 0x3FC00000 || id == 0x3FD40000;
}

void Obj::func_00124CF8()
{
    Model *model;
    unsigned int id;
    int mode;
    int r;
    int nine;
    unsigned int kind;

    model = x130->GetModel();
    x14C = &Obj::func_00125178;
    x154 = &Obj::func_00124CE8;
    x8 = 1;
    if (func_00222BD8(0, 1)) {
        x40 = 1;
        x44 = 2;
    } else {
        x44 = 1;
        x40 = 2;
    }
    nine = 9;
    x50 = 3;
    x54 = 8;
    x4C = nine;
    id = MODEL_ID(model);
    if (id == 0x3EF80000 && D_004B2840[0] != 20) {
        x48 = 22;
    } else if (id == 0x3F480000 && D_004B2840[0] != 20) {
        x48 = 22;
    } else if (id == 0x43800000 || id == 0x43940000 || id == 0x40D80000 || id == 0x3F840000
        || id == 0x3F980000) {
        x48 = 22;
    } else {
        x48 = nine;
    }
    xCC = func_00222B68(-0.34906584f, 0.34906584f);
    if (model != 0) {
        switch (D_004B2840[0]) {
        case 11:
            if (x130 != 0 && func_00192BA8((int)(func_002623C0(x130))) == 3) {
                x40 = 1;
                x44 = 1;
                x48 = 9;
            }
            SetScale(model, 1.4f);
            break;
        case 20:
            if (x130 != 0 && func_00192BA8((int)(func_002623C0(x130))) == 13) {
                if (func_00222BD8(0, 1)) {
                    x40 = 1;
                    x44 = 6;
                } else {
                    x40 = 1;
                    x44 = 1;
                }
            }
            SetScale(model, 1.4f);
            break;
        case 18:
            if (x130 == 0) break;
            mode = func_00192BA8((int)(func_002623C0(x130)));
            if (mode != 3) break;
            x40 = 1;
            r = func_00222BD8(0, 3);
            switch (r) {
            case 0: x44 = 5; break;
            case 1: x44 = 6; break;
            case 2: x44 = 7; break;
            case 3: x44 = 1; break;
            }
            kind = MODEL_ID(model);
            if (kind == 0x3FAC0000 || kind == 0x3FC00000 || kind == 0x3FD40000) {
                SetScale(model, 1.2f);
            }
            break;
        case 12:
            if (x130 != 0 && func_00192BA8((int)(func_002623C0(x130))) == 3) {
                x40 = 1;
                if (func_00222BD8(0, 1)) {
                    x44 = 1;
                } else {
                    x44 = 5;
                }
            }
            if (MODEL_ID(model) != 0x40D80000) {
                SetScale(model, 1.2f);
            }
            break;
        case 3:
            if (x130 != 0 && func_00192BA8((int)(func_002623C0(x130))) == 3) {
                x40 = 2;
                x44 = 2;
            }
            kind = MODEL_ID(model);
            if (kind == 0x3FAC0000 || kind == 0x3FC00000 || kind == 0x3FD40000) {
                SetScale(model, 1.2f);
            }
            break;
        case 5:
            x40 = 1;
            x44 = 1;
            SetScale(model, 1.4f);
            break;
        default:
            SetScale(model, 1.4f);
            break;
        }
    }
    x134 = 0;
}
