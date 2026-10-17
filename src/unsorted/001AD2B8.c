/* UNIT 0x001AD2B8 0x001AD580 */
#include "rider.h"
typedef struct Ctrl {
    int x0;
    unsigned char x4;
    char pad5[3];
    float x8;
    float xC;
    int x10;
    int x14;
    unsigned char x18;
    unsigned char x19;
    char pad1A[2];
    float x1C;
    float x20;
    float x24;
    float x28;
    float x2C;
    char pad30[4];
    float x34;
    float x38;
    char pad3C[0x54 - 0x3C];
    float x54;
    char pad58[0x68 - 0x58];
    char x68[0x40];
    char padA8[0x140 - 0xA8];
    Rider *body;
} Ctrl;

extern float func_00248EB8(float a, float b);
extern float func_002237F8(float a);
extern int func_00238F60(void);
extern float func_001ACE98(Ctrl *ctrl);
extern float func_00236088(float a, float b, float c, float d);
extern void func_001ACAE0(void *obj, float f);
extern void func_001ACB70(void *obj, float f);
extern int func_0012D300(Rider *rider);
extern float fabsf(float);

void func_001AD2B8(Ctrl *ctrl)
{
    float f20;
    float v1;
    float v2;
    float b;
    float t;
    float cur38;
    float flag;
    float zero;
    float d;
    float diff;
    void *p;
    AnimationHandle tmp;
    int r1;
    int idx2;
    char *part;

    if (ctrl->x18 != 0) {
        if (ctrl->x4 == 0) {
            f20 = 0.0f;
            if (ctrl->body->x7A78 != 0.0f) {
                f20 = func_002237F8(func_00248EB8(ctrl->x24 + ctrl->x24, 1.0f)) * ctrl->x54;
                if (ctrl->x1C < 0.0f) {
                    f20 = -f20;
                }
            }
            v1 = ((ctrl->x1C + f20) * 1.25f / ctrl->x28) * (float)func_00238F60();
            v2 = ctrl->x1C * ((float)func_00238F60() * 0.041666668f);
            b = func_001ACE98(ctrl);
            t = ctrl->x24 + (v2 * b + (1.0f - b) * v1);
            ctrl->x24 = t;
            if (t > 1.0f) {
                ctrl->x24 = t - 1.0f;
            } else if (t < 0.0f) {
                ctrl->x24 = t + 1.0f;
            }
            diff = ctrl->x24 - ctrl->x2C;
            d = fabsf(diff);
            /* Zero-byte labels (same device as src/unsorted/00174098.c): this one
               keeps diff live past the abs (so |diff| lands in its own register)
               and keeps the 0.05 load below the abs, as retail does. */
            __asm__ volatile ("L001AD2B8_d:" : : "f"(d), "f"(diff));
            if (d < 0.05f) {
                if (ctrl->x19 == 0) {
                    ctrl->x18 = 0;
                }
            }
            ctrl->x34 += 0.05f;
        }
    }
    flag = 0.0f;
    cur38 = ctrl->x38;
    if (ctrl->body->x7A78 != flag) {
        flag = 1.0f;
    }
    p = ctrl->x68;
    ctrl->x38 = func_00236088(cur38, flag, 0.1f, 0.05f);
    /* keeps the 0.0 set after the call above, as retail does */
    __asm__ volatile ("L001AD2B8_z:");
    zero = 0.0f;
    func_001ACAE0(p, ctrl->x24);
    func_001ACB70(p, zero);
    if (func_001ACE98(ctrl) > 0.0f) {
        part = (char *)ctrl->body + 0x1E40;
        r1 = func_0012D300(ctrl->body);
        part = part + 0x144 + r1 * 16;
        tmp = *(AnimationHandle *)part;
        func_00203B68(&tmp, 0, ctrl->x24);
        idx2 = func_0012D300(ctrl->body) + 19;
        tmp = ctrl->body->handles[idx2];
        func_00203B68(&tmp, 0, ctrl->x24);
    }
}
