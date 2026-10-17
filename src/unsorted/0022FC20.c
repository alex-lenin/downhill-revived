/* UNIT 0x0022FC20 0x0022FC60 */
typedef struct Slot {
    char pad0[0x4C];
    int used;
    char pad50[0x130 - 0x50];
} Slot;

extern Slot D_00734680[32];

/* Claim the first free slot, or return NULL when all 32 are in use */
Slot *func_0022FC20(void)
{
    Slot *slot;
    int i;

    for (i = 0; i < 32; i++) {
        slot = &D_00734680[i];
        if (slot->used == 0) {
            slot->used = 1;
            return slot;
        }
    }
    return 0;
}
