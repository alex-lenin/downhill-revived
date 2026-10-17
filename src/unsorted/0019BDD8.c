/* UNIT 0x0019BDD8 0x0019BE18 */
typedef struct Obj { char pad0[0x11]; signed char x11; signed char x12; signed char x13; char pad14[0x38]; int x4C; } Obj;
int func_0019BDD8(Obj *obj) {
    int result;
    if (obj->x13 == 0) return 6;
    if (obj->x4C != 0) return 4;
    if (obj->x11 != 0) result = obj->x12;
    else result = 5;
    return result;
}
