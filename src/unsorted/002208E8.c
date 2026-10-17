/* UNIT 0x002208E8 0x002209A8 */
typedef struct Entry {
    char pad0[0x4];
    unsigned char x4;
    unsigned char x5;
    unsigned char x6;
    unsigned char x7;
    char pad8[0x18];
} Entry;

extern unsigned char D_0029E2A8[];
extern Entry D_00660B80[];

int func_002208E8(int type, int index)
{
    int value = 0;

    if (D_0029E2A8[index] != 0) {
        switch (type) {
        case 0:
            value = (index + D_00660B80)->x4;
            break;
        case 1:
            value = (index + D_00660B80)->x5;
            break;
        case 2:
            value = (index + D_00660B80)->x6;
            break;
        case 3:
            value = (index + D_00660B80)->x7;
            break;
        }
    }
    return value;
}
