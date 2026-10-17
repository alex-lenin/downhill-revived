/* UNIT 0x00194E30 0x00194EF8 .rodata=0x28D920:0x28D960 */
extern int func_00194C80(void *a, int b);

int func_00194E30(void *a, unsigned int mode)
{
    int r;
    int result;

    r = func_00194C80(a, mode);
    result = 0;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        if (r == 0) {
            result = 0;
        } else if (r == 1) {
            result = 0;
        } else if (r == 2) {
            result = 2;
        } else if (r == 3) {
            result = 2;
        } else {
            result = 3;
        }
        break;
    case 3:
        result = r / 6;
        if (result == 4) result = 3;
        break;
    case 4:
        result = r / 3;
        if (result == 4) result = 3;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    default:
        break;
    }
    return result;
}

/* Same translation unit's .rodata: the "player%d" format used by
   func_001956C0/func_00195828/func_00195960 follows this jump table directly. */
const char D_0028D948[24] = "player%d";
