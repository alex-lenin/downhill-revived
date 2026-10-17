/* UNIT 0x00192F68 0x00193068 */
extern int D_0029E4A0;
extern int D_004B2828[];
extern void *D_0029E4C8;
extern char D_0038DCE0[];
extern char D_003F5440[];
extern char D_003F5690[];

extern void func_00164A78(char *p, void *value);
extern void func_00192B90(void *obj, void *value, int mode);
extern void func_00192990(void *obj, unsigned int *data);
extern void func_001933A0(void *handle);

/* msg points at a float tag; bits >> 18 is an entry id in [4000, 4500). */
void func_00192F68(unsigned int *msg)
{
    unsigned int bits;
    unsigned int top;
    unsigned int rem;

    if (D_0029E4A0 >= 200 || D_004B2828[0] >= 3)
        return;

    bits = *msg;
    top = bits & 0xFFFC0000;
    if (top == 0x3FDC0000) {
        func_00164A78(D_0038DCE0, msg);
        return;
    }
    rem = ((bits >> 18) - 4000) % 5;
    if (rem != 0) {
        func_00192B90(&D_003F5440[D_0029E4A0 * 0x250], msg, rem);
        return;
    }
    if (top == 0x44C00000) {
        D_0029E4C8 = msg;
        return;
    }
    func_00192990(&D_003F5690[D_0029E4A0 * 0x250], msg);
    func_001933A0(&D_003F5690[D_0029E4A0 * 0x250]);
    D_0029E4A0 = D_0029E4A0 + 1;
}
