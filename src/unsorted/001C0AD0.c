/* UNIT 0x001C0AD0 0x001C0B40 .rodata=0x28F8E0:0x28F910 */
extern int D_004B26AC[];
extern volatile int D_003817BC[];
int func_001C0AD0(void) {
    int value=D_004B26AC[0];
    unsigned int result=0xFFFFFFFF;
    if(value==20) value=D_003817BC[0];
    switch(value) {
    case 0: case 1: case 2: result=0; break;
    case 3: result=1; break;
    case 4: result=2; break;
    case 6: case 7: case 8: case 9: result=3; break;
    case 5: result=4; break;
    }
    return result;
}
