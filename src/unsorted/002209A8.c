/* UNIT 0x002209A8 0x00220AC0 */
extern unsigned char D_0029E2A8;
extern int D_00660D20[];
extern int func_002208E8(int axis, int port);

int func_002209A8(int dir, int port) {
    int result = 0;
    int x;
    int y;

    if (((unsigned char *)&D_0029E2A8)[port] != 0) {
        if (D_00660D20[port] != 4) {
            x = 0x80 - func_002208E8(2, port);
            y = 0x80 - func_002208E8(3, port);
            func_002208E8(0, port);
            func_002208E8(1, port);
            switch (dir) {
            case 0x80:
                result = x > 120;
                break;
            case 0x20:
                result = x < -120;
                break;
            case 0x10:
                result = y > 120;
                break;
            case 0x40:
                result = y < -120;
                break;
            }
        }
    }
    return result;
}
