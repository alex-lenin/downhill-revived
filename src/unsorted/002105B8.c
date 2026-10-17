/* UNIT 0x002105B8 0x002105E0 */
extern int D_00663B08[];
int func_002105B8(void) {
    int value = (D_00663B08[1] + 15) & ~15;
    D_00663B08[1] = value + 64;
    return value;
}
