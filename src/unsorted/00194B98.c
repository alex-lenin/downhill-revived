/* UNIT 0x00194B98 0x00194C00 */
struct Slot {
    char pad0[0x32];
    char x32;
    char pad33[0x44 - 0x33];
    int x44;
    int x48;
    int x4C;
};

struct SlotState {
    char pad0[0x10C];
    int x10C;
};

extern struct SlotState D_004B25A0;
extern void func_001952A0(struct Slot *slot);

void func_00194B98(struct Slot *slots)
{
    slots[D_004B25A0.x10C].x32 = 0;
    slots[D_004B25A0.x10C].x44 = 0;
    slots[D_004B25A0.x10C].x4C = slots[D_004B25A0.x10C].x48 = 0;
    func_001952A0(&slots[D_004B25A0.x10C]);
}
