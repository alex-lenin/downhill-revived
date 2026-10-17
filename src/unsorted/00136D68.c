/* UNIT 0x00136D68 0x00136E78 */
struct State {
    char pad0[0x119C]; int x119C;
    char pad11A0[0x1264 - 0x11A0]; int x1264;
    char pad1268[0x13DA - 0x1268]; unsigned char x13DA;
    char pad13DB[0x7A5C - 0x13DB]; int x7A5C;
};
struct Obj {
    char pad0[0x158];
    struct State *x158;
    char pad15C[0x478 - 0x15C];
    float x478;
    float x47C;
    float x480;
    char pad484[0x4A4 - 0x484];
    short x4A4;
    char pad4A6[0x4B0 - 0x4A6];
    long x4B0;
};

extern int func_00138560(struct Obj *);
extern void func_00136E78(struct Obj *);
extern void func_001350F0(struct Obj *);
extern float func_00235EA8(float cur, float target, float rate, float min);

void func_00136D68(struct Obj *obj)
{
    struct State *state;
    int active;

    if ((obj->x158->x7A5C ^ 1) != 0) return;
    if (!(obj->x4B0 & 4)) return;
    if (func_00138560(obj)) {
        if (obj->x47C > 0.01f) func_00136E78(obj);
        func_001350F0(obj);
        return;
    }
    if (obj->x4A4 >= 4) {
        func_00136E78(obj);
        return;
    }
    state = obj->x158;
    if (state->x13DA) active = state->x1264;
    else active = state->x119C;
    if (active) {
        if (obj->x478 > 0.01f) obj->x478 = 0.0f;
        obj->x47C = func_00235EA8(obj->x47C, obj->x480, 0.04f, 0.01f);
    } else {
        func_001350F0(obj);
    }
}
