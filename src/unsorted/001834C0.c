/* UNIT 0x001834C0 0x00183510 */
typedef struct Global { char pad[0x288]; int count; } Global;
extern Global D_004B25A0;
void func_001834C0(int *obj) {
 int i;
 obj[12]=0; obj[11]++;
 for(i=0;i<D_004B25A0.count;i++,obj++) { obj[0]=0; obj[4]=0; }
}
