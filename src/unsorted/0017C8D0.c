/* UNIT 0x0017C8D0 0x0017C940 */
struct Global { char pad[0x150]; char objects[4][0x1380]; };
extern struct Global D_003B2270[];
extern void func_00261D60(void *);
void func_0017C8D0(int enabled,int value) {
 int count, ptr, base;
 if(value == 0xFFFF && enabled) {
  count=3; base=(int)D_003B2270; ptr=base+0x150;
  do { count--; func_00261D60((void *)ptr); ptr+=0x1380; } while(count != -1);
 }
}
