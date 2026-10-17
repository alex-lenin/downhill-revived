/* UNIT 0x00182290 0x001822D8 .rodata=0x28C490:0x28C500 */
int func_00182290(int kind) {
    switch (kind) {
    case 0: case 1: case 2: case 3: case 5: case 11: case 12: case 19: case 20: return 2;
    case 6: case 7: case 8: case 9: case 14: case 15: case 17: case 25: return 0;
    case 4: case 10: case 13: case 16: case 18: case 22: case 24: return 1;
    default: return 3;
    }
}
