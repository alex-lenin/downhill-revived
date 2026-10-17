/* UNIT 0x001A0858 0x001A08E0 */
typedef unsigned int u128 __attribute__((mode(TI)));
extern char D_004B2A90[];
extern void func_001C57A8(void *, u128);
void func_001A0858(signed char *obj) {
 if (obj[0x518] == 2 && obj[0x519] >= obj[0x51A]) obj[0x518] = (unsigned char)obj[0x518] + 2;
 else obj[0x518] = (unsigned char)obj[0x518] + 1;
 if (obj[0x518] >= 6) obj[0x518] = 1;
 func_001C57A8(D_004B2A90, (u128)0x5458454E5F53ULL);
}
