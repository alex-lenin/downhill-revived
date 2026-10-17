/* UNIT 0x00113170 0x001132B0 */
typedef struct Body {
    char pad0[0x18];
    float x18;
} Body;

typedef struct Rider {
    char pad0[0x11E8];
    int x11E8;
    char pad11EC[0x124C - 0x11EC];
    int x124C;
    char pad1250[0x13D4 - 0x1250];
    unsigned char x13D4;
    char pad13D5[0x40C0 - 0x13D5];
    char x40C0[0x792C - 0x40C0];
    Body *x792C;
} Rider;

typedef struct Obj {
    Rider *x0;
    char pad4[4];
    Rider *x8;
    char padC[0x440 - 0xC];
    long x440;
} Obj;

extern float func_001385A8(void *);

void func_00113170(Obj *obj) {
    int active;

    obj->x440 &= ~(1L << 34);
    obj->x440 &= ~(1L << 35);
    obj->x440 &= ~(1L << 38);
    if (obj->x8 == 0)
        return;
    if (!(func_001385A8(obj->x8->x40C0) > 0.2f))
        return;
    if (!(obj->x8->x792C->x18 - obj->x0->x792C->x18 > 120.0f))
        return;
    if (!(func_001385A8(obj->x0->x40C0) > 0.5f)) {
        if (!(func_001385A8(obj->x0->x40C0) > 0.2f))
            return;
        active = obj->x0->x13D4 ? obj->x0->x124C : obj->x0->x11E8;
        if (active == 0)
            return;
    }
    obj->x440 |= 1L << 38;
}
