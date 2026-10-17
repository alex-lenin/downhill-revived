/* UNIT 0x00199030 0x001990B8 */
typedef struct Obj {
    char pad0[0x2010];
    unsigned int x2010;
    unsigned int x2014;
    char pad2018[0x8];
    unsigned int x2020;
    unsigned int x2024;
    char pad2028[0x14];
    float x203C;
    float x2040;
    float x2044;
    float x2048;
    char pad204C[0x14];
    int x2060;
} Obj;

extern void func_00199118(Obj *obj, int arg);

void func_00199030(Obj *obj)
{
    func_00199118(obj, 0);
    obj->x203C = 1.0f;
    obj->x2040 = 0.5f;
    obj->x2044 = 1.0f;
    obj->x2048 = 0.0008f;
    obj->x2020 = 0xFF20FF80;
    obj->x2024 = 0x2020FF80;
    obj->x2010 = 0x20FFFF80;
    obj->x2014 = 0x20202080;
    obj->x2060 = 0;
}
