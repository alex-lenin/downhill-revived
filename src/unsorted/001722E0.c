/* UNIT 0x001722E0 0x00172330 */
struct Entry { char pad[0x88]; int x88; char pad8C[0xC]; int x98; char pad9C[0x174]; };
extern struct Entry D_003AAEE0[];
void func_001722E0(int enabled, int mode) {
    int i;
    if (mode == 0xFFFF && enabled != 0) {
        for (i = 19; i != -1; i--) {
            D_003AAEE0[19-i].x88 = 0;
            D_003AAEE0[19-i].x98 = 0;
        }
    }
}
