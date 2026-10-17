/* UNIT 0x002204A8 0x00220550 */
extern unsigned char D_0029E2A8;
extern int func_00220AE0(int, int);
typedef union Packed { int value; struct { unsigned int b0:8, b1:8, b2:8, b3:8; } bytes; } Packed;
int func_002204A8(int index) {
    Packed result;
    result.value = 0;
    if (*(unsigned char *)(index + (int)&D_0029E2A8)) {
        result.bytes.b0 = func_00220AE0(2,index);
        result.bytes.b1 = func_00220AE0(3,index);
        result.bytes.b2 = func_00220AE0(0,index);
        result.bytes.b3 = func_00220AE0(1,index);
    }
    return result.value;
}
