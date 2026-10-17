/* UNIT 0x0023DDE0 0x0023DED0 */
extern int D_0029E450;
extern int D_0075A9D8[];
extern void func_0023C8D8(int a, int b);

void func_0023DDE0(int toggle)
{
    if (D_0029E450 < 3) {
        if (toggle) {
            if (D_0029E450 == 1) {
                D_0029E450 = 2;
                func_0023C8D8(3, 0);
                func_0023C8D8(4, 1);
            } else {
                D_0029E450 = 1;
                func_0023C8D8(0, 0);
            }
        } else if (D_0075A9D8[0] == 1) {
            func_0023C8D8(3, 0);
            func_0023C8D8(4, 1);
        } else if (D_0075A9D8[0] == 2) {
            func_0023C8D8(4, 0);
            func_0023C8D8(3, 1);
        } else if (D_0075A9D8[0] == 3) {
            func_0023C8D8(1, 0);
            func_0023C8D8(2, 1);
        } else if (D_0075A9D8[0] == 4) {
            func_0023C8D8(2, 0);
            func_0023C8D8(1, 1);
        }
    }
}
