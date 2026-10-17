/* First word of .sdata: defined (initialised) in this TU, so gp-relative under -G8. */
int D_29C100 = 0;
extern int D_29E700[];
void func_10A208(int x) { D_29E700[D_29C100] = x; D_29C100++; }
