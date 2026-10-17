/* UNIT 0x001BFFF0 0x001C0180 */
int func_001BFFF0(void *p,int value) {
 switch(value) {
 case 0x20:return 0;
 case 0x40:return 1;
 case 0x60:case 0x1E0:return 2;
 case 0x80:return 3;
 case 0xA0:return 4;
 case 0xC0:return 5;
 case 0xE0:return 8;
 case 0x100:return 9;
 case 0x120:return 7;
 case 0x140:return 10;
 case 0x160:case 0x1C0:return 6;
 case 0x180:return 11;
 case 0x1A0:return 12;
 case 0x200:return 16;
 default:return 13;
 }
}
