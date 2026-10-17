/* UNIT 0x001ACFA8 0x001AD098 */
#include "camera.h"
typedef struct Body {
    char pad0[0x3530];
    char x3530[0x40C0 - 0x3530];
    char x40C0[0x7908 - 0x40C0];
    int x7908;
} Body;

typedef struct Ctrl {
    int x0;
    char pad4[0x19 - 0x4];
    unsigned char x19;
    char pad1A[0x34 - 0x1A];
    float x34;
    float x38;
    char pad3C[0x140 - 0x3C];
    Body *body;
} Ctrl;

extern void func_001AD1C0(Ctrl *ctrl);
extern void func_001AD2B8(Ctrl *ctrl);

extern float func_001AD760(Ctrl *ctrl, float a, float b);
extern float func_001385A8(void *p);
extern void func_001AD098(Ctrl *ctrl);

void func_001ACFA8(Ctrl *ctrl)
{
    Body *body;
    char *target;
    float t;
    float r;

    if (ctrl->body) {
        switch (ctrl->x0) {
        case 0:
            func_001AD1C0(ctrl);
            break;
        case 1:
            ctrl->x19 = 0;
            break;
        }
        func_001AD2B8(ctrl);
        func_001B3028((RiderCamera *)(ctrl->body->x3530), 2, ctrl->x38);
        body = ctrl->body;
        if (body->x7908 >= 35) {
            if (body->x7908 < 38) goto skip;
        }
        target = body->x3530;
        t = func_001AD760(ctrl, 4.0f, 0.0f);
        r = func_001385A8(ctrl->body->x40C0);
        func_001B3028((RiderCamera *)(target), 3, -t * ctrl->x34 * (1.0f - r));
    skip:
        func_001AD098(ctrl);
    }
}
