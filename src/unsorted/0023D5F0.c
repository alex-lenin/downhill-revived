/* UNIT 0x0023D5F0 0x0023D608 */
struct Entry {
    char pad0[0x360];
};

extern struct Entry D_0075BAC0[];

struct Entry *func_0023D5F0(int index)
{
    return &D_0075BAC0[index];
}
