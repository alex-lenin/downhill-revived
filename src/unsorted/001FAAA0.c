/* UNIT 0x001FAAA0 0x001FABE8 */
struct Obj {
    char pad0[0x30];
    int x30;
    int x34;
    int x38;
    float x3C;
    float x40;
    float x44;
    float x48;
    float x4C;
    float x50;
};

extern float D_00277770[];
extern float D_002777A0[];
extern float D_00277800[];
extern float D_002778D0[];
extern float D_00277968[];
extern float D_00277A00[];
extern float D_00277A98[];

extern int func_001FAA18(int id);

void func_001FAAA0(struct Obj *obj)
{
    float a;
    float b;

    obj->x3C = D_00277770[obj->x38] + 0.85f + D_002778D0[obj->x30];
    obj->x40 = D_002777A0[obj->x38] + 0.75f + D_00277968[obj->x30];
    obj->x44 = D_00277A98[obj->x30];
    obj->x48 = D_00277800[obj->x34] * D_00277A00[obj->x30];
    obj->x4C = 1.0f;
    a = 1.0f;
    if (func_001FAA18(obj->x30)) {
        a = 1.09f;
    }
    if (func_001FAA18(obj->x30)) {
        b = 1.1f;
    } else {
        b = 1.0f;
    }
    obj->x50 = a * b;
}
