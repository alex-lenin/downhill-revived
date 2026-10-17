/* UNIT 0x001C1B48 0x001C1C00 .rodata=0x28FA80:0x28FAC0 */
extern int D_004B26AC[];
extern int D_0039E7F4[];

extern void func_001C1C00(void *obj, int id);

void func_001C1B48(void *obj)
{
    int mode = D_004B26AC[0];

    switch (mode) {
    case 0:
    case 1:
    case 2:
        if (D_0039E7F4[0] == 2 && mode == 0) {
            func_001C1C00(obj, 0x52);
        } else {
            func_001C1C00(obj, 0x2C);
        }
        break;
    case 3:
        func_001C1C00(obj, 0x31);
        break;
    case 4:
        func_001C1C00(obj, 0x4F);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 12:
    case 13:
        func_001C1C00(obj, 0x32);
        break;
    }
}
