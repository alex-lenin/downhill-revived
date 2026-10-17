/* UNIT 0x0011A548 0x0011A5E8 */
typedef struct Global { char pad[0x288]; int x288; } Global;
extern int D_0043EEC8[];
extern Global D_004B25A0;
extern void func_0011A5E8(void *,int,void *);
void func_0011A548(void *obj,void *arg) {
 int i; int *p;
 if(D_0043EEC8[0]>=421) {
 i=0;
 if(D_004B25A0.x288>0) {
 p=(int *)((char *)obj+0x50130);
 do { func_0011A5E8(obj,*p,arg); i++; p++; } while(i<D_004B25A0.x288);
 }
 }
}
