/* UNIT 0x00220230 0x00220268 */
typedef struct { short x0, x2; char pad4[0x1c]; } Entry;
extern unsigned char D_0029E2A8[];
extern Entry D_00660B80[];
int func_00220230(int index) {
    int result = 0;
    if (D_0029E2A8[index]) result = ~D_00660B80[index].x2;
    return result;
}
