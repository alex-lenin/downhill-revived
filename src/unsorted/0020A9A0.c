/* UNIT 0x0020A9A0 0x0020AAC8 */
struct Obj { int x0; short x4,x6; char pad8[24]; unsigned int *x20,*x24; unsigned int x28[1]; };
extern void func_0020A5E0(unsigned int*,int);
void func_0020A9A0(struct Obj *obj,int arg) {
 int x,y;
 if(obj) {
 for(x=0;x<obj->x4;x++) {
 for(y=0;y<obj->x6;y++) {
 unsigned int packed=((struct Obj *)((int)obj+((y*obj->x4+x)<<2)))->x28[0];
 unsigned int *p=obj->x20+(packed>>10),*end=p+(packed&1023);
 while(p<end) {
 unsigned int value=*p++;
 func_0020A5E0(obj->x24+(value>>7),arg);
 }
 }
 }
 }
}
