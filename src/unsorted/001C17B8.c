/* UNIT 0x001C17B8 0x001C18A0 .rodata=0x28F990:0x28F9D0 */
extern int D_004B26AC[];

extern void func_001C18A0(void *p, int id, int arg);

void func_001C17B8(void *p, int arg, int flag)
{
    switch (D_004B26AC[0]) {
    case 0:
    case 1:
    case 2:
        if (flag)
            func_001C18A0(p, 0x2D, arg);
        else
            func_001C18A0(p, 0x55, arg);
        break;
    case 3:
        if (flag)
            func_001C18A0(p, 0x30, arg);
        else
            func_001C18A0(p, 0x2F, arg);
        break;
    case 4:
        if (flag)
            func_001C18A0(p, 0x50, arg);
        else
            func_001C18A0(p, 0x53, arg);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 12:
    case 13:
        func_001C18A0(p, 0x2E, arg);
        break;
    }
}
