/* UNIT 0x002079B0 0x002079C8 */
struct D_002794A8_t {
    int x0;
    int x4;
    char pad8[0x488 - 0x8];
    int x488;
};

extern struct D_002794A8_t D_002794A8;

void func_002079B0(int arg)
{
    D_002794A8.x0 = 0;
    D_002794A8.x4 = arg;
    D_002794A8.x488 = 0;
}
