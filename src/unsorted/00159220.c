/* UNIT 0x00159220 0x00159318 */
typedef struct Slot {
    char pad0[0x200];
} Slot;

typedef struct Obj {
    int x0;
    char pad4[0xC];
    Slot x10[20];
    int x2810[20];
    int x2860[20];
    float x28B0;
    float x28B4;
    float x28B8;
    float x28BC;
    float x28C0;
    float x28C4;
    float x28C8;
    int x28CC;
    int x28D0[20];
    int x2920;
    int x2924;
    int x2928;
    int x292C;
    int x2930;
    int x2934;
    int x2938;
} Obj;

extern void func_00159788(Slot *);

void func_00159220(Obj *obj)
{
    int i;

    obj->x0 = 0;
    for (i = 0; i < 20; i++) {
        func_00159788(&obj->x10[i]);
        obj->x2810[i] = 0;
        obj->x2860[i] = 0;
        obj->x28D0[i] = 0;
    }
    obj->x28B0 = 0.48888889f;
    obj->x28B4 = 0.97777778f;
    obj->x28B8 = 1.5f;
    obj->x28BC = 2.5f;
    obj->x28C0 = 0.2f;
    obj->x28C4 = 2.0f;
    obj->x28C8 = 7.0f;
    obj->x292C = 11;
    obj->x2920 = 0;
    obj->x2924 = 0;
    obj->x2928 = 0;
    obj->x28CC = 0;
    obj->x2930 = 0;
    obj->x2934 = 0;
    obj->x2938 = 1;
}
