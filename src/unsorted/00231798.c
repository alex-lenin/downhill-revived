/* UNIT 0x00231798 0x00231828 */
extern void func_0025A700(int);
extern void func_0025A720(int);
extern int D_0029E348[2],D_0029E370[];
extern int D_0029E358,D_0029E380;
int func_00231798(void) {
 int next;
 func_0025A700(D_0029E348[D_0029E358]);
 next=D_0029E358==0;
 D_0029E380++;
 D_0029E358=next;
 func_0025A720(D_0029E348[next]);
 return D_0029E370[D_0029E358];
}
