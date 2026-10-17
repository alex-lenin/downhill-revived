/* UNIT 0x001821E8 0x00182290 .rodata=0x28C460:0x28C490 */
extern int D_0026F0F0[], D_0026F110[], D_0026F130[], D_0026F150[], D_0026F1B8[], D_0026F1F0[], D_0076F9B0[];
int func_001821E8(int type, int index) {
 int value = 0;
 switch (type) {
 case 0: value = D_0026F0F0[index]; break;
 case 1: value = D_0026F110[index]; break;
 case 2: value = D_0026F130[index]; break;
 case 3: value = D_0026F150[index]; break;
 case 4: value = D_0026F1B8[index]; break;
 case 5: value = D_0026F1F0[index]; break;
 case 6: case 7: case 8: case 9: value = D_0076F9B0[index]; break;
 }
 return value;
}
