/* UNIT 0x001A05E0 0x001A0608 */
struct Unk0043EEC0 {
    char pad0[0x188];
    int x188;
    char pad18C[0xC];
    int x198;
};

extern struct Unk0043EEC0 D_0043EEC0;

void func_001A05E0(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_0043EEC0.x188 = 0;
        D_0043EEC0.x198 = 0;
    }
}
