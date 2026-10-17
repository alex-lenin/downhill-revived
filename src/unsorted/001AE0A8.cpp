/* UNIT 0x001AE0A8 0x001AE1D0 */
typedef struct Info { char pad0[0x5]; unsigned char x5; } Info;

typedef struct Rider {
    char pad0[0x3514]; Info *x3514;
    char pad3518[0x7A78 - 0x3518]; float x7A78;
} Rider;

typedef struct Obj {
    int x0; float x4, x8, xC;
    char pad10[0x14]; float x24; float x28;
    char pad2C[0x4]; char x30[0x64];
    char x94[0x64];
    Rider *xF8;
} Obj;

extern "C" int func_00130010(Rider *);
extern "C" void func_001ACAE0(void *, float);
extern "C" void func_001ACB70(void *, float);
extern "C" float func_00236088(float, float, float, float);

static inline float Min(float a, float b) { return a <? b; }

extern "C" void func_001AE0A8(Obj *obj)
{
    if (obj->xF8->x3514->x5 || func_00130010(obj->xF8)) {
        func_001ACAE0(obj->x30, Min(obj->x8 + obj->xC, 1.0f) * -0.5f + 0.5f);
        func_001ACB70(obj->x30, 0.0f);
        func_001ACAE0(obj->x94, obj->x24 * -0.4f + 0.5f);
        func_001ACB70(obj->x94, 0.0f);
        if (obj->xF8->x7A78 == 0.0f)
            obj->x28 = func_00236088(obj->x28, obj->x4, 0.2f, 0.1f);
        else
            obj->x28 = func_00236088(obj->x28, 0.0f, 0.2f, 0.1f);
    }
}
