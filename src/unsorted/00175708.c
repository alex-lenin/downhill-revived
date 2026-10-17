/* UNIT 0x00175708 0x00175730 */
typedef struct Obj {
    char pad0[0x12C4];
    int x12C4;
    char pad12C8[0x4];
    int x12CC;
    char pad12D0[0x94];
    int x1364;
} Obj;

void func_00175708(Obj *obj, int value)
{
    int t;

    obj->x1364 = value;
    t = obj->x12CC ? 120 : 90;
    obj->x12C4 = t - (t >> 2);
}
