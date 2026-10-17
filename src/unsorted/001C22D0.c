/* UNIT 0x001C22D0 0x001C23A0 */
struct Obj {
    float x0;
    float x4;
    float x8;
    float xC;
    int x10;
    float x14;
    float x18;
    float x1C;
    int x20;
    float x24;
};

extern float func_00222A68(float, float);
extern int func_00238F48(void);

void func_001C22D0(struct Obj *p)
{
    p->x10 = (p->x10 == 1) ? (p->x10 = 0) : (p->x10 = 1);
    p->x14 = func_00222A68(p->x0, p->x4);
    p->x20 = func_00238F48();
    p->x18 = func_00222A68(p->x8, p->xC);
    if (p->x10 == 0) {
        p->x18 = -p->x18;
    }
    if (p->x1C == 0.0f) {
        p->x24 = (p->x18 * 0.5f) / (p->x14 * 60.0f);
    } else {
        p->x24 = p->x18 / (p->x14 * 60.0f);
    }
}
