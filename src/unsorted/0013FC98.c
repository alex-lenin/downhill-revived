/* UNIT 0x0013FC98 0x0013FD00 */
extern int D_0026E420[];
extern void func_0018E398(void *);
extern void func_0018E408(void *,int,int);
void func_0013FC98(void *o) {
 int count=15; int *p;
 func_0018E398(o);
 p=D_0026E420;
 do { func_0018E408(o,*p,1); p++; } while(--count>=0);
}
