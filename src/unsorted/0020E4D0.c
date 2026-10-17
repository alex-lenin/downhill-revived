/* UNIT 0x0020E4D0 0x0020E520 */
extern int func_00247D28(int a, int b, int c, char *d);
extern void func_002474E0(int a);
extern char D_0029E180;

int func_0020E4D0(long a, long b, int c)
{
    int ret = func_00247D28(b, a, c, &D_0029E180);
    func_002474E0(0);
    return ret;
}
