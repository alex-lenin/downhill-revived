/* UNIT 0x00223B50 0x00223B90 */
extern char D_00663B08[];
extern char D_007A5080[];
extern void func_00223A98(void *, void *, int);
void func_00223B50(int enabled, int code) {
    if (code == 0xFFFF && enabled) {
        func_00223A98(D_00663B08, D_007A5080, 0xFA000);
    }
}
