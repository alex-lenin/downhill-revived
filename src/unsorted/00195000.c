/* UNIT 0x00195000 0x00195040 */
typedef struct Obj { char pad0[0x320]; int x320[33]; int x3A4[33]; signed char x428[1]; } Obj;
int func_00195000(Obj *obj, int index, int lower, int upper, int value) {
    int flags = 0;
    if (lower < obj->x320[index]) flags |= 1;
    if (value < obj->x428[index]) flags |= 2;
    if (obj->x3A4[index] < upper) flags |= 4;
    return flags;
}
