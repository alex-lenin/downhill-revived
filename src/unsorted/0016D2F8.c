/* UNIT 0x0016D2F8 0x0016D350 .rodata=0x28AFD0:0x28B010 */
extern void func_0016EB40(void *);
extern void func_0016ED38(void *);
void func_0016D2F8(int *obj) {
 switch(*obj) {
 case 1: case 3: case 5: case 8: case 10: case 12: func_0016EB40(obj); break;
 case 2: case 6: case 7: case 11: func_0016ED38(obj); break;
 case 0: case 4: case 9: case 13: case 14: default: break;
 }
}
