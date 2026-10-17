/* UNIT 0x0023DF40 0x0023DF58 */
typedef struct Entry0C0 {
    char pad0[0xC0];
} Entry0C0;

extern Entry0C0 D_0027ECC0[];

Entry0C0 *func_0023DF40(int index) {
    return &D_0027ECC0[index];
}
