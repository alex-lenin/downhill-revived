/* UNIT 0x0019E818 0x0019E898 */
struct Obj { char pad[0x4a8]; int x4a8; };
extern char D_0043DD38[], D_0043E280[];
extern int func_0019E518(void *);
extern void func_0019C868(void *,int,int,int,int,float,float,float,float);
extern void func_00187548(struct Obj *);
void func_0019E818(struct Obj *o) {
 void *p=D_0043DD38;
 int n=func_0019E518(D_0043E280);
 func_0019C868(p,n,127,1,0,64.0f,64.0f,512.0f,336.0f);
 o->x4a8=5;
 func_00187548(o);
}
