/* UNIT 0x00194C98 0x00194CF0 */
struct Slot {
    char pad0[0x32];
    signed char x32;
    char pad33[0x50 - 0x33];
};

extern int func_001822D8(int index);

float func_00194C98(struct Slot *slots, int index)
{
    int n = slots[index].x32;

    return (float)(n + 1) / (float)func_001822D8(index);
}
