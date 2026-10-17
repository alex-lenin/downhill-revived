/* UNIT 0x001C3350 0x001C33D0 .rodata=0x28FCA0:0x28FD10 */
extern int D_004B2840;
char *func_001C3350(char *ptr) {
    switch (D_004B2840) {
    case 1:
    case 7:
    case 13:
        ptr -= 0x20;
        break;
    case 2:
    case 8:
    case 10:
        ptr -= 0x40;
        break;
    case 3:
    case 4:
    case 9:
        ptr -= 0x60;
        break;
    case 5:
    case 14:
    case 22:
        ptr -= 0x80;
        break;
    case 11:
    case 15:
    case 16:
        ptr -= 0xA0;
        break;
    case 12:
    case 21:
    case 26:
        ptr -= 0xC0;
        break;
    case 17:
    case 18:
    case 20:
        ptr -= 0xE0;
        break;
    case 19:
    case 25:
        ptr -= 0x100;
        break;
    case 23:
        ptr -= 0x120;
        break;
    case 0:
    case 6:
    case 24:
    default:
        break;
    }
    return ptr;
}
