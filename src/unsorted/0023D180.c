/* UNIT 0x0023D180 0x0023D1A0 */
typedef struct Entry {
    int x0;
    int x4;
    char pad8[0xE8];
} Entry;

extern Entry D_00528360[];

int func_0023D180(int i)
{
    Entry *e = &D_00528360[i];
    return e->x4;
}
