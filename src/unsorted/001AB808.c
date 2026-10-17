/* UNIT 0x001AB808 0x001AB8A8 */
struct Entry {
    void *x0;
    char pad4[0x2C];
};

struct Slot {
    char pad0[0x40];
    char x40[0x100];
};

extern char D_00287E58[];
extern struct Entry D_004A4190[];
extern struct Slot D_004A5990[];
extern void func_002318C0(void *);

void func_001AB808(int enable, int id)
{
    struct Entry *entry;
    struct Slot *slot;
    int i;
    int j;

    if (id == 0xFFFF && enable != 0) {
        entry = D_004A4190;
        i = 128;
        while (i--) {
            entry->x0 = D_00287E58;
            entry++;
        }
        slot = D_004A5990;
        j = 8;
        while (j--) {
            func_002318C0(slot->x40);
            slot++;
        }
    }
}
