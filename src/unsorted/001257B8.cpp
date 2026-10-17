/* UNIT 0x001257B8 0x001258A0 */
struct Transform {
    char pad0[0x40];
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
    char pad0[0x14];
    int x14;
    char pad18[0x130 - 0x18];
    Part *x130;
    char pad134[0x180 - 0x134];
    char x180[0x60];
    char x1E0[0x10];
    char x1F0[0x10];
};

extern "C" {
void func_001258A0(Obj *obj, float dt);
float func_00261540(void *a, void *b, void *c, void *out);
float func_00238F40(void);
void func_00236558(void *a, void *b, float t);
void func_00124300(Obj *obj);
void func_00123F60(Obj *obj, int a);
void func_00123DB0(Obj *obj, void *m);
void func_00211280(Transform *t);
}

extern "C" void func_001257B8(Obj *obj, float dt)
{
    float tmp[4] __attribute__((aligned(16)));
    float speed;

    func_001258A0(obj, dt);
    speed = func_00261540(obj->x180, obj->x1E0, obj->x1F0, tmp);
    func_00236558(obj->x1E0, tmp, func_00238F40());
    if (speed > 4.0f || obj->x14 != 0) {
        func_00124300(obj);
    } else {
        func_00123F60(obj, 0);
    }
    func_00123DB0(obj, obj->x130->GetTransform()->x40);
    func_00211280(obj->x130->GetTransform());
}
