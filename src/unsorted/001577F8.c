/* UNIT 0x001577F8 0x00157890 */
struct Src {
    long flags;
    char pad8[0x8];
    char x10[0x30];
    char x40[0x50];
    char x90[0x20];
    char xB0[0x10];
    char xC0[0x10];
};

struct Sub {
    float x0;
    char pad4[0x2C];
    void *x30;
    void *x34;
    void *x38;
    void *x3C;
    void *x40;
};

struct Obj {
    int x0;
    char pad4[0xC];
    struct Sub x10;
    char pad54[0x11C - 0x54];
    struct Src *x11C;
    float x120;
    float x124;
    float x128;
    float x12C;
};

void func_001577F8(struct Obj *obj, int value) {
    struct Src *src = obj->x11C;
    struct Sub *sub;

    if (src) {
        obj->x0 = value;
        obj->x124 = 0.63f;
        sub = &obj->x10;
        sub->x34 = src->x10;
        sub->x38 = src->x40;
        if (src->flags & 0x0400000000000000) {
            sub->x3C = src->x90;
            sub->x40 = src->xC0;
            sub->x30 = src->xB0;
        }
        {
            float b = 0.4f;
            obj->x12C = 0.18f;
            obj->x128 = b;
            obj->x120 = 1.0f;
            sub->x0 = 1.0f;
        }
    }
}
