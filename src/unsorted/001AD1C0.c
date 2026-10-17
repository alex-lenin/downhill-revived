/* UNIT 0x001AD1C0 0x001AD2B8 */
typedef struct Body {
    char pad0[0x40C0];
    char x40C0[0x7A78 - 0x40C0];
    float x7A78;
} Body;

typedef struct Ctrl {
    char pad0[0x19];
    unsigned char x19;
    char pad1A[0x3C - 0x1A];
    float x3C;
    float x40;
    char pad44[0x4C - 0x44];
    float x4C;
    float x50;
    char pad54[0x140 - 0x54];
    Body *body;
} Ctrl;

extern float func_001385A8(void *p);
extern float func_001ACE98(Ctrl *ctrl);
extern void func_001AD580(Ctrl *ctrl, int mode, float a, float b);
extern void func_001AD700(Ctrl *ctrl, float value);
extern void func_001AD710(Ctrl *ctrl);

void func_001AD1C0(Ctrl *ctrl)
{
    float value;
    float base;

    if (ctrl->body == 0) return;

    if (ctrl->body->x7A78 != 0.0f) {
        value = ctrl->x3C + (ctrl->x40 - ctrl->x3C) * func_001385A8(ctrl->body->x40C0);
        value = value * 0.5f + func_001385A8(ctrl->body->x40C0) * 0.5f;
        base = func_001ACE98(ctrl);
        value = base + (1.0f - base) * value;
        func_001AD700(ctrl, value * ctrl->x4C);
        if (value > 0.0f) {
            func_001AD710(ctrl);
        }
    } else {
        func_001AD580(ctrl, 1, ctrl->x50, 1.0f);
        ctrl->x19 = 0;
    }
}
