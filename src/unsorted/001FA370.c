/* UNIT 0x001FA370 0x001FA3E8 */
extern char D_002761E8[], D_002764A8[], D_00276DF8[];
void *func_001FA370(int kind, int index) {
    switch (kind) {
    case 1: return D_00276DF8 + index * 30;
    case 2: return D_002764A8 + index * 40;
    case 3: return D_002761E8 + index * 32;
    default: return 0;
    }
}
