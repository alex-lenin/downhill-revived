/* UNIT 0x0020EE10 0x0020EE28 */
struct Entry {
    char pad0[0x8];
    float x8;
    char padC[0x14];
};

extern struct Entry D_004F3220[];

float func_0020EE10(int i)
{
    struct Entry *e = &D_004F3220[i];
    return e->x8;
}
