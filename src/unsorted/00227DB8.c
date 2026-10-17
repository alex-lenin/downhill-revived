/* UNIT 0x00227DB8 0x00227DD0 */
struct Entry {
    char pad0[0x40];
};

extern struct Entry D_0027BFC0[];

struct Entry *func_00227DB8(int i)
{
    return &D_0027BFC0[i];
}
