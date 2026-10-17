/* UNIT 0x00196970 0x001969C8 */
typedef struct Slot {
    void *x0;
    char pad4[0x10];
} Slot;

typedef struct SlotTable {
    char pad0[0x18];
    Slot slots[150];
} SlotTable;

extern SlotTable D_004348F0;
extern char D_002873D0[];

void func_00196970(int enable, int id)
{
    SlotTable *table;
    Slot *slot;
    int i;

    if (id == 0xFFFF && enable != 0) {
        table = &D_004348F0;
        slot = table->slots;
        i = 150;
        while (i-- != 0) {
            slot->x0 = D_002873D0;
            slot++;
        }
    }
}
