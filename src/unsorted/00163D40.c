/* UNIT 0x00163D40 0x00163D70 */
typedef struct Item { char pad0[0x20]; } Item;
typedef struct Obj { Item *x0; char pad4[0x58]; int count; } Obj;
Item *func_00163D40(Obj *obj, int index) {
    if (index < 0 || index >= obj->count) index = 0;
    return obj->x0 + index;
}
