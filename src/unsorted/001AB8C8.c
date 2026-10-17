/* UNIT 0x001AB8C8 0x001AB950 */
typedef struct Obj {
    char pad0[0x1690];
    unsigned int x1690;
    unsigned int x1694;
    char pad1698[0x8];
    unsigned int x16A0;
    unsigned int x16A4;
    char pad16A8[0x14];
    float x16BC;
    float x16C0;
    float x16C4;
    float x16C8;
    char pad16CC[0x14];
    int x16E0;
} Obj;

extern void func_001AB9B0(Obj *obj, int arg);

void func_001AB8C8(Obj *obj)
{
    func_001AB9B0(obj, 0);
    obj->x16BC = 1.0f;
    obj->x16C0 = 0.5f;
    obj->x16C4 = 1.0f;
    obj->x16C8 = 0.0008f;
    obj->x16A0 = 0xFF20FF80;
    obj->x16A4 = 0x2020FF80;
    obj->x1690 = 0x20FFFF80;
    obj->x1694 = 0x20202080;
    obj->x16E0 = 0;
}
