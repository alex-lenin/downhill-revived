/* UNIT 0x00220D70 0x00220D88 */
struct Entry {
    char pad0[0xC0];
};

extern struct Entry D_00660E80[];

struct Entry *func_00220D70(int i)
{
    return &D_00660E80[i];
}
