/* UNIT 0x0023D098 0x0023D0B0 */
struct Entry {
    char pad0[0xF0];
};

extern struct Entry D_00528380[];

struct Entry *func_0023D098(int index)
{
    return &D_00528380[index];
}
