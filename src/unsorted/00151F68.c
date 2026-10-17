/* UNIT 0x00151F68 0x00151FA8 */
extern int D_0029E4B0;
extern char D_003830B0[];
void func_00151F68(void) {
 char *p;
 D_0029E4B0=0;
 p=D_003830B0;
 do { *(int *)(p+4)=0; *(int *)(p+8)=0; p+=0x30; } while ((int)p < (int)(D_003830B0+0x960));
}
