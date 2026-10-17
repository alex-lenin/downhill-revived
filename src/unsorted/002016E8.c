/* UNIT 0x002016E8 0x00201710 */
extern int *D_00279428[];
extern int D_0027942C[];

void func_002016E8(int *buf, int size)
{
    D_0027942C[0] = size;
    D_00279428[0] = buf;
    buf[size + 1] = 0;
    buf[0] = 0;
}
