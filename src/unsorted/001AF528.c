/* UNIT 0x001AF528 0x001AF5E0 */
struct Obj { int x0,x4; char *x8; char xC[100]; char x70[1]; };
extern void func_001AF5E0(struct Obj *);
extern void func_001AC7B0(void *,void *);
extern void func_001AC810(void *,float);
extern void func_001ACB70(void *,float);
void func_001AF528(struct Obj *p,char *r) {
 void *a,*b;
 if(r) p->x8=r;
 a=p->xC;
 func_001AF5E0(p);
 b=p->x70;
 func_001AC7B0(a,p->x8+0x340);
 func_001AC810(a,0.0f);
 func_001ACB70(a,1.8f);
 func_001AC7B0(b,p->x8+0x2194);
 func_001AC810(b,0.0f);
 func_001ACB70(b,1.8f);
}
