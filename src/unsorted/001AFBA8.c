/* UNIT 0x001AFBA8 0x001AFC90 */
struct Rider {
    char pad0[0x4414];
    unsigned char x4414;
    char pad4415[0x7F];
    unsigned char x4494;
    char pad4495[0xA37];
    char *x4ECC;
    char pad4ED0[0x1440];
    char x6310[1];
};

struct Anim {
    char pad0[0x44];
    float x44;
};

struct Obj {
    int x0;
    int x4;
    struct Rider *x8;
    struct Anim xC;
    char pad5C[0xF4 - 0x5C];
    unsigned char xF4;
};

extern void func_001AFB88(struct Obj *obj);
extern int func_00238F48(void);
extern float func_001ACBA0(struct Anim *anim);
extern void func_001AC880(struct Anim *anim, float value);
extern float func_001AFDE8(struct Obj *obj);

void func_001AFBA8(struct Obj *obj)
{
    struct Rider *rider = obj->x8;
    struct Anim *anim;
    float t;

    if (rider == 0) {
        return;
    }
    if (rider->x4ECC == rider->x6310) {
        func_001AFB88(obj);
        return;
    }
    if (rider->x4414 == 2 || rider->x4494 == 2) {
        if (obj->x0 != 2 && obj->x4 == -1) {
            obj->x4 = func_00238F48();
        }
        obj->x0 = 2;
    } else {
        obj->x0 = 0;
    }
    anim = &obj->xC;
    t = func_001ACBA0(anim);
    if (t >= 1.0f) {
        func_001AC880(anim, 0.0f);
    } else {
        anim->x44 = func_001AFDE8(obj);
    }
    /* Never true (xF4 is a byte), but combine only removes it after flow has
       seen t live past the calls, which is why retail keeps t in $f20. */
    if ((obj->xF4 & 0x100) && t > 0.5f) {
        obj->x0 = 1;
    }
}
