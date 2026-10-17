/* UNIT 0x001E3938 0x001E3980 */
extern char D_004C37A0[], D_004C3D00[], D_004C4210[];
extern int func_001E20E0(void *);
extern int func_001E3618(void *);
extern int func_001E30A8(void *);
void func_001E3938(int arg0, int arg1) {
    if (arg1 == 0xFFFF) {
        if (arg0 != 0) {
            func_001E20E0(D_004C37A0);
            func_001E3618(D_004C3D00);
            func_001E30A8(D_004C4210);
        }
    }
}
