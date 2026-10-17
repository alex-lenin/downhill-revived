/* UNIT 0x00112758 0x00112808 */
typedef struct Inner {
    char pad0[0x796C];
    int x796C;
} Inner;

/* Mode values step by 0x20; the cases that leave value unchanged are
   needed for gcc to build retail's comparison tree. */
int func_00112758(Inner **obj, int unused, int value) {
    switch ((*obj)->x796C) {
    case 0x60:
    case 0x80:
    case 0xE0:
    case 0x100:
    case 0x120:
    case 0x140:
    case 0x180:
    case 0x1A0:
    case 0x1C0:
    case 0x1E0:
        break;
    case 0xA0:
    case 0x160:
        value *= 3;
        break;
    case 0x40:
    case 0xC0:
        value = 0;
        break;
    }
    return value;
}
