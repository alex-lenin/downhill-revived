/* UNIT 0x00125E38 0x00126238 */
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

struct Transform {
    char pad0[0x10];
    Vec4 x10;
};

struct Kind {
    unsigned int flags;
};

struct Shape {
    Kind *kind;
};

class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Transform *GetTransform();
};

class Model : public Part {
public:
    Shape *x4;
    char pad8[0x24C - 0x8];
    char x24C;
};

struct Body {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x30];
    Vec4 x50;
};

struct Rider {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    Body *x792C;
    char pad7930[0x7A68 - 0x7930];
    unsigned char x7A68;
};

struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x10];
    int x1C;
    char pad20[0x48];
    char x68[0x70];
    float xD8;
    char padDC[0x4];
    Vec4 xE0;
    char padF0[0x40];
    Model *x130;
    char pad134[0xC];
    void *x140;
    Rider *x144;
};

extern "C" {
extern const float D_0029C208[];
extern Rider *D_0043EEC0[];
int func_00122D10(Obj *, float);
void func_00126B20(Obj *, int);
void func_00123760(Obj *, void *);
float func_001385A8(void *);
void func_00123788(Obj *, Vec4 *);
float func_001ACBA0(void *);
float func_00235EA8(float, float, float, float);
void func_001ACAE0(void *, float);
void func_00123F60(Obj *, int);
void func_00126238(Obj *, Vec4 *);
void func_00123AD0(Obj *, Vec4 *, int);
void func_001238B8(Obj *);
}

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VSUB(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b))
#define VMADD(dst, a, b, f) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tmfc1 $2, %3\n\tqmtc2.ni $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmaddx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a), "r"(b), "f"(f) : "$2")

static inline float SquaredLength(Vec4 *v)
{
    float result;

    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3"
        : "=f"(result) : "f"(v->x), "f"(v->y), "f"(v->z));
    return result;
}

static inline void UpdateSpeed(Obj *obj)
{
    func_001ACAE0(obj->x68, func_00235EA8(func_001ACBA0(obj->x68), 0.5f, 0.1f, 0.0001f));
}

extern "C" void func_00125E38(Obj *obj)
{
    Vec4 tmp;
    Vec4 diff;
    float distance;
    void *target;
    Rider *rider;
    Rider *other;

    if (func_00122D10(obj, 200.0f) == 0) {
        if (obj->x8 != 0) {
            func_00126B20(obj, 0);
            obj->x130->x24C = 1;
            obj->x144 = 0;
        }
        return;
    }
    distance = D_0029C208[0];
    obj->x130->x24C = 0;
    if (obj->x1C >= 61) {
        if (obj->x8 != 1) {
            func_00126B20(obj, 1);
        }
    } else {
        target = obj->x140;
        distance = obj->xD8;
        COPY_VECTOR(tmp, &obj->xE0);
        func_00123760(obj, target);
        rider = D_0043EEC0[0];
        if (rider != 0 && rider->x7A68 != 0 && func_001385A8(rider->x40C0) < 0.1f) {
            if ((obj->x130->x4->kind->flags >> 18) == 0x1018) {
                if (obj->x8 != 1) {
                    func_00126B20(obj, 1);
                }
            } else if (obj->x8 != 2 && distance > 150.0f) {
                func_00126B20(obj, 2);
            }
        } else {
            other = obj->x144;
            if (other != 0 && distance < 144.0f && (obj->x130->x4->kind->flags >> 18) != 0x1018) {
                if (obj->x8 != 4) {
                    func_00126B20(obj, 4);
                }
            } else if (obj->x8 != 1) {
                if (other != 0) {
                    if (distance > 576.0f) {
                        func_00126B20(obj, 1);
                    }
                } else {
                    func_00126B20(obj, 1);
                }
            }
        }
    }
    if (obj->x8 == 0) {
        func_00126B20(obj, 1);
    }
    switch (obj->x8) {
    case 0:
        break;
    case 1:
        if (distance < 40000.0f) {
            func_00123788(obj, &tmp);
        }
        UpdateSpeed(obj);
        func_00123F60(obj, 0);
        break;
    case 2:
        if (D_0043EEC0[0] != 0) {
            Body *body = D_0043EEC0[0]->x792C;

            VMADD(tmp, &body->x10, &body->x50, 3.0f);
            VSUB(diff, &tmp, &obj->x130->GetTransform()->x10);
            distance = SquaredLength(&diff);
        }
        if (distance < 100.0f) {
            func_00126B20(obj, 1);
        } else {
            func_00126238(obj, &tmp);
        }
        func_00123788(obj, &tmp);
        UpdateSpeed(obj);
        break;
    case 4:
        func_00123AD0(obj, &tmp, 0);
        func_001238B8(obj);
        break;
    }
}
