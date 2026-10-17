/* UNIT 0x001ACCA8 0x001ACCC8 */
typedef struct Obj {
    char pad0[0x38];
    int x38;
    char pad3C[0x54 - 0x3C];
    float x54;
    char pad58[0x68 - 0x58];
    int x68;
    char pad6C[0x140 - 0x6C];
    int x140;
} Obj;

void func_001ACCA8(Obj *obj)
{
    obj->x54 = 0.1f;
    obj->x68 = 0;
    obj->x38 = 0;
    obj->x140 = 0;
}
