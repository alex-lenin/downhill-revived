/* UNIT 0x00220130 0x00220180 */
extern int D_00660CA0[];
extern int D_00660CC0[];
extern char D_00660E10[][6];
extern void func_00245688(int a, int b, char *c);

void func_00220130(int i)
{
    func_00245688(D_00660CA0[i], D_00660CC0[i], D_00660E10[i]);
}
