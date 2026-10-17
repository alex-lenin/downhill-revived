/* UNIT 0x001B4000 0x001B4050 .sdata=0x29D3FC:0x29D400 */
int D_0029D3FC = 0;
extern char D_004A9E40[];
extern void func_001B20F8(void *, int);
extern void func_00230DA8(void *);
void func_001B4000(int value, int update) {
    D_0029D3FC = value;
    func_001B20F8(D_004A9E40, value);
    if (update) func_00230DA8(D_004A9E40);
}
