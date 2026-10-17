/* UNIT 0x0011C378 0x0011C400 */
typedef struct Globals { char pad[0x2180]; int flags; } Globals;
extern Globals D_004325A0;
void func_0011C378(void) {
 int i=0;
 for(i=0;i<16;i++) {
  int bit;
  int select; int value;
  select=0;
  value=(i+1)<<5;
  if(value==0xE0 || value==0x100 || value==0x140 || value==0x180 || value==0x1A0) select=1;
  bit=1<<i;
  if(select !=0) D_004325A0.flags |= bit;
 }
}
