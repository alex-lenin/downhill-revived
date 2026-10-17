/* UNIT 0x00126898 0x001269B8 */
struct Vec4 { float x, y, z, w; } __attribute__((aligned(16)));

struct Transform {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x20];
    char x40[0x40];
};

class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Transform *GetTransform();
};

struct Obj {
    char pad0[0x2C];
    int x2C;
    char pad30[0x130 - 0x30];
    Part *x130;
    char pad134[0x1D0 - 0x134];
    char x1D0[0x10];
    Vec4 x1E0;
    char x1F0[0x10];
};

extern "C" {
void func_001258A0(Obj *obj, float dt);
float *func_00261020(void *a);
void func_002361D8(void *a, void *b, void *c, void *out);
float func_00238F40(void);
void func_00236558(void *a, void *b, float t);
void func_00123F60(Obj *obj, int a);
void func_00123DB0(Obj *obj, void *m);
void func_00211280(Transform *t);
}

extern "C" void func_00126898(Obj *obj)
{
    float tmp[4] __attribute__((aligned(16)));
    float speed;
    Vec4 *pos;
    Transform *t;

    speed = 11.7333336f;
    if (obj->x2C != 0) {
        speed = 1.17333341f;
    }
    func_001258A0(obj, speed);
    *func_00261020(obj->x1D0) = speed;
    pos = &obj->x1E0;
    func_002361D8(obj->x1D0, pos, obj->x1F0, tmp);
    func_00236558(pos, tmp, func_00238F40());
    func_00123DB0(obj, obj->x130->GetTransform()->x40);
    t = obj->x130->GetTransform();
    /* 128-bit vector copy (lq/sq through $2), as elsewhere in the game */
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(t->x10) : "r"(pos) : "$2");
    func_00123F60(obj, 0);
    func_00211280(obj->x130->GetTransform());
}
