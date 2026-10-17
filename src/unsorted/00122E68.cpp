/* UNIT 0x00122E68 0x001232D0 .rodata=0x288D00:0x288D60 .sdata=0x29C1B8:0x29C1C8 */
#include "model.h"
struct Vec {
    float x, y, z;
    void Set(float a, float b, float c) { x = a; y = b; z = c; }
};
struct Body { long x0; char pad8[0x28]; Vec x30; };
class Actor {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual Body *v4();
};
struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x34];
    int x40;
    int x44;
    int x48;
    int x4C;
    char pad50[0x7C];
    float xCC;
    char padD0[0x60];
    Actor *x130;
    int x134;
    char pad138[0x14];
    int (Obj::*x14C)();
    int (Obj::*x154)();
    int func_00123350();
    int func_00122E58();
    void func_00122E68();
};
struct Global { char pad[0x2A0]; int x2A0; };
extern Global D_004B25A0;


extern "C" int func_00222BD8(int, int);
extern "C" void func_001232D0(Obj *);
extern "C" float func_00222B68(float, float);

void Obj::func_00122E68()
{
    this->x14C = &Obj::func_00123350;
    this->x154 = &Obj::func_00122E58;
    this->x8 = 1;
    this->x40 = 1;
    switch (D_004B25A0.x2A0) {
    case 19:
    case 25:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 16) {
            this->x44 = 2;
            this->x130->v4()->x0 |= 0x0100000000000000L;
            this->x130->v4()->x30.Set(1.4f, 1.4f, 1.4f);
        } else {
            func_001232D0(this);
        }
        break;
    case 20:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 14) {
            switch (func_00222BD8(0, 1)) {
            case 0: this->x44 = 5; break;
            case 1: this->x44 = 6; break;
            }
        } else if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 12) {
            if (func_00222BD8(0, 1)) this->x44 = 1;
            else this->x44 = 5;
            this->x130->v4()->x0 |= 0x0100000000000000L;
            this->x130->v4()->x30.Set(1.4f, 1.4f, 1.4f);
        } else if (this->x130 && (**(int **)this->x130->v4() & 0xFFFC0000) == 0x3EE40000) {
            if (func_00222BD8(0, 1)) this->x44 = 1;
            else this->x44 = 5;
        } else {
            func_001232D0(this);
        }
        break;
    case 18:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 11) {
            switch (func_00222BD8(0, 3)) {
            case 0: this->x44 = 5; break;
            case 1: this->x44 = 6; break;
            case 2: this->x44 = 7; break;
            case 3: this->x44 = 1; break;
            }
        } else {
            func_001232D0(this);
        }
        break;
    case 12:
    case 21:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 11) {
            if (func_00222BD8(0, 1)) this->x44 = 1;
            else this->x44 = 5;
        } else if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 9) {
            if (func_00222BD8(0, 1)) this->x44 = 1;
            else this->x44 = 2;
        } else {
            func_001232D0(this);
        }
        break;
    case 5:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 12) {
            if (func_00222BD8(0, 1)) this->x44 = 1;
            else this->x44 = 2;
            this->x130->v4()->x0 |= 0x0100000000000000L;
            this->x130->v4()->x30.Set(1.4f, 1.4f, 1.4f);
        } else if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 14) {
            switch (func_00222BD8(0, 3)) {
            case 0: this->x44 = 5; break;
            case 1: this->x44 = 6; break;
            case 2: this->x44 = 7; break;
            case 3: this->x44 = 1; break;
            }
        } else {
            func_001232D0(this);
        }
        break;
    case 2:
    case 8:
    case 10:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 7) {
            this->x44 = 26;
        } else {
            func_001232D0(this);
        }
        break;
    case 3:
        if (this->x130 && func_00192BA8((int)(func_002623C0(this->x130))) == 11) {
            this->x40 = 2;
            this->x44 = 2;
        } else {
            func_001232D0(this);
        }
        break;
    default:
        func_001232D0(this);
        break;
    }
    this->x48 = 3;
    this->x4C = 8;
    this->xCC = func_00222B68(-0.34906584f, 0.34906584f);
    this->x134 = 0;
}
