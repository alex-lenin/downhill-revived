/* UNIT 0x00240080 0x00240270 */
extern unsigned long D_002995B8[];
#define STEP crc=table[((int)crc ^ *buf++) & 0xff] ^ (crc >> 8)
unsigned long func_00240080(unsigned long crc, const unsigned char *buf, long len) {
 unsigned long *table;
 if(!buf) return 0;
 table=D_002995B8;
 crc ^= 0xffffffffUL;
 while(len>=8) {STEP;STEP;STEP;STEP;STEP;STEP;STEP;STEP;len-=8;}
 if(len) do {STEP;}while(--len);
 return crc^0xffffffffUL;
}
