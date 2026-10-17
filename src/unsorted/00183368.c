/* UNIT 0x00183368 0x001833A8 */
struct Global { char pad0[0x288]; int x288; };
extern struct Global D_004B25A0;
void func_00183368(int *p) {
 int count;
 p[12]=0;
 count=0;
 if(D_004B25A0.x288>0) {
 do { p[0]=0; count++; p[4]=0; p++; } while(count<D_004B25A0.x288);
 }
}
