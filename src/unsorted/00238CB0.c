/* UNIT 0x00238CB0 0x00238D10 */
extern int func_00238F10(void), func_00243CE8(int);
extern int D_0029E3E4, D_0029E3E8, D_0029E3EC;
int func_00238CB0(int *out) {
 int time = func_00238F10();
 D_0029E3EC = time;
 D_0029E3E4 = time - D_0029E3E8;
 if (out) *out = func_00243CE8(0) == 0;
 else func_00243CE8(0);
 return 1;
}
