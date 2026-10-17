/* UNIT 0x0022FAE8 0x0022FC20 */
typedef struct Slot {
    char pad00[0x0C];
    float x0C;
    char pad10[0x0C];
    float x1C;
    int x20;
    int x24;
    int x28;
    float x2C;
    int x30;
    int x34;
    int x38;
    int x3C;
    int x40;
    float x44;
    int x48;
    int x4C;
    int x50;
    float x54;
    float x58;
    float x5C;
    float x60;
    float x64;
    float x68;
    float x6C;
    float m70[16];
    float mB0[16];
    float mF0[16];
} Slot;

extern Slot D_00734680[32];
extern void func_00221F20(float *);
extern void func_00221930(float *, const float *, const float *);

void func_0022FAE8(int idx)
{
    float m0[16];
    float m1[16];
    float m2[16];
    float tmp[16];
    Slot *s;
    float t;

    func_00221F20(m0);
    m0[3] = 0.5f;
    m0[7] = 0.5f;
    func_00221F20(m1);
    m1[0] = 0.5f;
    m1[5] = 0.5f;
    func_00221F20(m2);
    s = &D_00734680[idx];
    t = (s->x60 - s->x64) * 0.5f;
    m2[0] = 1.0f / (t / s->x54);
    t = (s->x68 - s->x6C) * 0.5f;
    m2[5] = 1.0f / (t / s->x54);
    m2[10] = -s->x58 / (s->x5C - s->x58);
    m2[11] = (s->x58 * s->x5C) / (s->x5C - s->x58);
    m2[14] = -1.0f / s->x54;
    m2[15] = 0.0f;
    func_00221930(tmp, m0, m1);
    func_00221930(D_00734680[idx].m70, tmp, m2);
}
