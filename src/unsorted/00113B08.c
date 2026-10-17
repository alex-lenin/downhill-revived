/* UNIT 0x00113B08 0x00113D10 */
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Rider {
    char pad0[0x40];
    Vec x40;
    Vec x50;
    Vec x60;
} Rider;

typedef struct Game {
    char pad0[0x4180];
    Vec x4180;
    char pad4190[0x4564 - 0x4190];
    short x4564;
    char pad4566[0x792C - 0x4566];
    Rider *x792C;
} Game;

typedef struct Obj {
    Game *x0;
    char pad4[0x250 - 0x4];
    float x250;
    char pad254[0x260 - 0x254];
    float x260;
    char pad264[0x2D4 - 0x264];
    int x2D4;
} Obj;

typedef struct Frame {
    char pad0[0x10];
    Vec x10;
    Vec x20;
} Frame;

typedef struct Sub40C0 {
    char pad[0x4A4];
    short x4A4;
} Sub40C0;

extern float func_001385A8(void *);
extern Frame *func_00138558(void *);

static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

int func_00113B08(Obj *obj) {
    float lim1;
    float lim2;
    int ok;
    int t;
    char *p;
    float d1;
    float d2;
    float a1;
    float a2;
    float x;
    float y;
    float z;
    float len;
    float inv;
    float t250;
    float a250;
    float k250;
    float t260;
    float a260;
    float k260;
    Vec tmp;
    Rider *r;
    Rider *r2;
    Frame *fr;
    Vec *gp;

    ok = 0;
    t = obj->x2D4;
    if (t == 0) {
        lim1 = 120.0f;
        lim2 = 0.7f;
        ok = 1;
    } else if (t == 1) {
        lim1 = 120.0f;
        lim2 = 0.7f;
        ok = 1;
    } else {
        lim1 = 1.0f;
        lim2 = 0.9f;
        if (func_001385A8((char *)obj->x0 + 0x40C0) > 0.3f) {
            t250 = obj->x250;
            a250 = __builtin_fabsf(t250);
            __asm__ volatile ("Lb08_k1:" : : "f"(t250));
            k250 = 0.05235988f;
            __asm__ volatile ("Lb08_k1u:" : : "f"(k250));
            if (a250 < k250) {
                t260 = obj->x260;
                a260 = __builtin_fabsf(t260);
                __asm__ volatile ("Lb08_k2:" : : "f"(t260));
                k260 = 0.12217305f;
                __asm__ volatile ("Lb08_k2u:" : : "f"(k260));
                if (a260 < k260) {
                    ok = 1;
                }
            }
        }
    }
    if (ok) {
        p = (char *)obj->x0 + 0x40C0;
        ok = 0;
        if ((float)((Sub40C0 *)p)->x4A4 < lim1) {
            r = obj->x0->x792C;
            fr = func_00138558(p);
            d1 = Dot(&r->x60, &fr->x20);
            a1 = __builtin_fabsf(d1);
            __asm__ volatile ("L3b08_a:" : : "f"(d1), "f"(a1));
            if (a1 > lim2) {
                gp = &obj->x0->x4180;
                x = gp->x;
                y = gp->y;
                z = gp->z;
                __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                    : "=f"(len) : "f"(x), "f"(y), "f"(z));
                __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
                tmp.x = x * inv;
                tmp.y = y * inv;
                tmp.z = z * inv;
                r2 = obj->x0->x792C;
                d2 = Dot(&tmp, &r2->x40);
                a2 = __builtin_fabsf(d2);
                __asm__ volatile ("L3b08_b:" : : "f"(d2), "f"(a2));
                if (a2 < 0.2f) {
                    ok = 1;
                }
            }
        }
    }
    return ok;
}
