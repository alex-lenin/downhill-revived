/* UNIT 0x0023CF30 0x0023CF48 */
struct Entry {
    char pad0[0x40];
};

extern struct Entry D_0075B1F0[];

struct Entry *func_0023CF30(int index)
{
    return &D_0075B1F0[index];
}
