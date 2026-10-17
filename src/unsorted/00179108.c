/* UNIT 0x00179108 0x00179130 */
typedef struct Obj {
    char pad0[0x12A4];
    int x12A4;
    char pad12A8[0x8];
    int x12B0;
    char pad12B4[0x8];
    int x12BC;
} Obj;

void func_00179108(Obj *obj, int state, int arg)
{
    if (obj->x12A4 == 0 || obj->x12A4 == 47) {
        obj->x12A4 = state;
        obj->x12B0 = arg;
        obj->x12BC = 0;
    }
}
