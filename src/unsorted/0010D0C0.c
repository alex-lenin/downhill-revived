/* UNIT 0x0010D0C0 0x0010D650 */
typedef struct Body {
    char pad0[0x18];
    float x18;
} Body;

typedef struct Rider {
    char pad0[0x11E4];
    int x11E4[3];
    char pad11F0[0x1248 - 0x11F0];
    int x1248[3];
    char pad1254[0x13D3 - 0x1254];
    unsigned char x13D3[3];
    char pad13D6[0x40C0 - 0x13D6];
    char x40C0[0x792C - 0x40C0];
    Body *x792C;
    char pad7930[0x7964 - 0x7930];
    short x7964;
    short x7966;
    short x7968;
    short x796A;
    char pad796C[0x7A69 - 0x796C];
    unsigned char x7A69;
    char pad7A6A[0x7A78 - 0x7A6A];
    float x7A78;
} Rider;

typedef struct Sub {
    char pad0[0x68];
    float x68;
    char pad6C[0xB0 - 0x6C];
    int xB0;
} Sub;

typedef struct Obj {
    Rider *x0;
    Rider *x4;
    Rider *x8;
    char padC[0x2D4 - 0xC];
    Sub x2D4;
    char pad388[0x400 - 0x388];
    int x400;
    char pad404[0x440 - 0x404];
    unsigned long x440;
} Obj;

#define BIT(n) ((unsigned long)1 << (n))

extern float func_001385A8(void *);
extern int func_0010D650(Obj *);
extern int func_00238F60(void);
extern void func_00112E80(Obj *);
extern int func_001118A8(Obj *);

static inline Sub *get_sub(Obj *obj)
{
    return &obj->x2D4;
}

static inline int is_active(Rider *r)
{
    return r->x7A78 != 0.0f;
}

static inline int option(Rider *r, int i)
{
    return r->x13D3[i] ? r->x1248[i] : r->x11E4[i];
}

void func_0010D0C0(Obj *obj)
{
    float dz;
    float diff;
    float speed;
    int checkAhead;
    int checkBehind;
    int limit;
    int threshold;
    int found;

    obj->x440 |= BIT(35);
    obj->x440 |= BIT(34);
    obj->x440 &= ~BIT(38);
    if (obj->x4 != 0 && func_001385A8(obj->x4->x40C0) > 0.01f) {
        checkAhead = 0;
        found = func_0010D650(obj);
        checkBehind = 0;
        if (found) {
            dz = obj->x8->x792C->x18 - obj->x0->x792C->x18;
            if (obj->x440 & BIT(49)) {
                if (dz < 0.0f) {
                    obj->x440 &= ~BIT(49);
                }
            } else if (dz > 30.0f) {
                obj->x440 |= BIT(49);
            }
            if (obj->x440 & BIT(48)) {
                if (dz > get_sub(obj)->x68) {
                    obj->x440 &= ~BIT(48);
                }
            } else if (dz < -10.0f) {
                obj->x440 |= BIT(48);
            }
            if (dz > 0.0f) {
                obj->x400 += func_00238F60();
                diff = func_001385A8(obj->x0->x40C0) - func_001385A8(obj->x4->x40C0);
                if (diff > 0.07f) {
                    dz += (diff - 0.07f) * 250.0f;
                }
            } else {
                obj->x400 = 0;
            }
            if (dz > get_sub(obj)->x68) {
                if (dz > 100.0f) {
                    speed = func_001385A8(obj->x0->x40C0);
                    if (speed > 0.5f || (speed > 0.25f && option(obj->x0, 1))) {
                        obj->x440 |= BIT(38);
                    }
                } else {
                    checkAhead = 0;
                    checkBehind = 0;
                }
            } else if (dz > 0.0f) {
                if (obj->x440 & BIT(48)) {
                    obj->x440 |= BIT(34);
                    if (func_001385A8(obj->x0->x40C0) < func_001385A8(obj->x4->x40C0)) {
                        checkBehind = 1;
                    }
                } else if (obj->x440 & BIT(49)) {
                    checkAhead = 0;
                    checkBehind = 0;
                } else {
                    checkAhead = is_active(obj->x4);
                    checkBehind = obj->x4->x7A69;
                }
            } else {
                checkAhead = 1;
                checkBehind = 1;
            }
        }
        limit = get_sub(obj)->xB0;
        if (obj->x0->x792C->x18 < obj->x8->x792C->x18) {
            limit *= 3;
        }
        if (checkAhead) {
            if (obj->x4->x7A78 != 0.0f && limit < obj->x4->x7966) {
                obj->x440 |= BIT(34);
            } else if (option(obj->x0, 0) && limit >= obj->x4->x796A) {
                obj->x440 |= BIT(34);
            } else {
                obj->x440 &= ~BIT(34);
            }
        } else if (option(obj->x0, 0) && limit >= obj->x0->x7966) {
            obj->x440 |= BIT(34);
        } else {
            obj->x440 &= ~BIT(34);
        }
        if (checkBehind) {
            if (obj->x4->x7A69 && limit < obj->x4->x7964) {
                goto set;
            }
            if (!option(obj->x0, 2)) {
                goto clear;
            }
            threshold = obj->x4->x7968;
        } else {
            if (!option(obj->x0, 2)) {
                goto clear;
            }
            threshold = obj->x0->x7964;
        }
        if (limit >= threshold) {
        set:
            obj->x440 |= BIT(35);
        } else {
        clear:
            obj->x440 &= ~BIT(35);
        }
    } else {
        func_00112E80(obj);
        obj->x440 = (obj->x440 & ~BIT(35)) | ((unsigned long)(func_001118A8(obj) & 1) << 35);
    }
}
