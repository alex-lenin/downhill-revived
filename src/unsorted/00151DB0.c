/* UNIT 0x00151DB0 0x00151DF0 */
extern int D_0029E4AC;
extern char D_00381DF0[];
void func_00151DB0(void) {
 char *p;
 D_0029E4AC=0;
 p=D_00381DF0;
 do { *(int *)(p+4)=0; *(int *)(p+8)=0; p+=0x30; } while ((int)p < (int)(D_00381DF0+0x12C0));
}
