/* UNIT 0x0023D688 0x0023D6A0 */
struct Entry {
    char pad0[0x40];
};

extern struct Entry D_0075B1F0[];

struct Entry *func_0023D688(int index)
{
    return &D_0075B1F0[index];
}
