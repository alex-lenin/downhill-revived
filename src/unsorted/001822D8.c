/* UNIT 0x001822D8 0x00182330 .rodata=0x28C500:0x28C540 */
extern int D_0029E4B8;

int func_001822D8(unsigned kind)
{
    int result = 0;
    switch (kind) {
    case 0:
    case 1:
    case 2:
        result = 7;
        break;
    case 3:
        result = 24;
        break;
    case 4:
        result = 13;
        break;
    case 5:
        result = 6;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        result = D_0029E4B8;
        break;
    }
    return result;
}

/* Preserve the adjacent literal so jump-table section padding stays unchanged. */
const char D_0028C528[24] __attribute__((aligned(8))) = "LOADBAR1";
