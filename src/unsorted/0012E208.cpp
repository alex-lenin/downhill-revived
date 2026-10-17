/* UNIT 0x0012E208 0x0012E598 .sdata=0x29C290:0x29C298 */
struct Vec { float x, y, z, w; };
struct Rider {
    char pad0[0x7960];
    float x7960;
    char pad7964[0x7A5C - 0x7964];
    int x7A5C;
};
struct Global {
    char pad0[8];
    int x8;
    char padC[0x488 - 0xC];
    int x488;
};
class Target {
public:
    virtual void v0();
    virtual void v1(Vec *, Rider *);
};
extern Global D_0043EEC0;
float D_0029C290 = 15.0f;
extern "C" int func_0012E598(Rider *, Vec *, Rider **);
extern "C" int func_0012E780(Rider *, Vec *, Target **);
extern "C" void func_0012E940(Rider *, int, Vec *, Rider *);

typedef struct Obj208 { char pad0[0x7960]; float x7960; } Obj208;
extern "C" int func_0012E208(Obj208 *obj) {
    int result = 0;
    if (obj->x7960 < D_0029C290) result = 1;
    return result;
}

static inline float vec_len(float x, float y, float z) {
    float len;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(len) : "f"(x), "f"(y), "f"(z));
    return len;
}

extern "C" void func_0012E230(Rider *r, int mode, int flag) {
    Vec v;
    Rider *foundRider;
    Target *foundTarget;

    if ((unsigned)(D_0043EEC0.x488 - 2) >= 3)
        return;
    if (D_0043EEC0.x8 < 0x12D && r->x7A5C != 1)
        return;
    r->x7960 = D_0029C290 + D_0029C290;
    if (func_0012E598(r, &v, &foundRider)) {
        float f3, f5, x;
        f3 = 6.0f;
        f5 = 4.0f;
        if (r->x7A5C == 1) {
            f3 = 6.0f * 1.3f;
            f5 = 4.0f * 1.3f;
        }
        if (mode == 5 || mode == 0 || mode == 6 || mode == 1) {
            f3 *= 1.3f;
            f5 *= 1.3f;
        } else if (mode == 8 || mode == 3) {
            f3 *= 1.5f;
            f5 *= 1.5f;
        }
        x = v.x;
        if (x > 0.0f) {
            if (x < f3 && __builtin_fabsf(v.y) < f5) {
                float len = vec_len(x, v.y, v.z);
                r->x7960 = len;
                if (flag != 0 && (unsigned)(mode - 5) < 4) {
                    r->x7960 = vec_len(x, v.y, v.z);
                    func_0012E940(foundRider, mode, &v, r);
                }
            }
        } else {
            float len = vec_len(x, v.y, v.z);
            r->x7960 = len;
            if (flag != 0 && -f3 < x && __builtin_fabsf(v.y) < f5 && (unsigned)mode < 4) {
                func_0012E940(foundRider, mode, &v, r);
            }
        }
    } else if (func_0012E780(r, &v, &foundTarget) && flag != 0) {
        float f2, f3, x;
        f2 = 12.0f;
        f3 = 10.0f;
        if (r->x7A5C == 1) {
            f2 = 12.0f * 1.3f;
            f3 = 10.0f * 1.3f;
        }
        if (mode == 5 || mode == 0 || mode == 6 || mode == 1) {
            f2 *= 1.3f;
            f3 *= 1.3f;
        } else if (mode == 8 || mode == 3) {
            f2 *= 1.5f;
            f3 *= 1.5f;
        }
        x = v.x;
        if (x > 0.0f) {
            if (x < f2) {
                x = __builtin_fabsf(v.y);
                if (x < f3 && (unsigned)(mode - 5) < 4) {
                    foundTarget->v1(&v, r);
                }
            }
        } else {
            if (-f2 < x) {
                x = __builtin_fabsf(v.y);
                if (x < f3 && (unsigned)mode < 4) {
                    foundTarget->v1(&v, r);
                }
            }
        }
    }
}
