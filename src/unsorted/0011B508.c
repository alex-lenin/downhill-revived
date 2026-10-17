/* UNIT 0x0011B508 0x0011B588 */
typedef struct { char pad[0x440]; unsigned long flags; } Item;
typedef struct { char pad[0x78D4]; Item *x78D4; char pad2[0x184]; int x7A5C; char pad3[0x170]; } Row;
typedef struct { char pad[0x501A8]; int count; } Obj;
void func_0011B508(Obj *obj, int enabled) {
    Row *row = (Row *)obj;
    int i = 0;
    if (obj->count > 0) {
        unsigned long bits;
        enabled = (enabled != 0);
        bits = (unsigned long)enabled << 33;
        do {
            if (row->x7A5C == 3 && row->x78D4) {
                row->x78D4->flags = (row->x78D4->flags & ~((unsigned long)1 << 33)) | bits;
            }
            i++;
            row++;
        } while (i < obj->count);
    }
}
