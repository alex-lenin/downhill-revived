/* UNIT 0x0015C288 0x0015C440 .sdata=0x29C700:0x29C708 */
struct Input { char *x0; virtual void a(); virtual void b(); virtual int test(); };
struct Obj { char *x0; int x4; int x8; };
struct Vector { float x,y,z,w; };
float D_0029C700 = 0.5f;
float D_0029C704 = 2.0f;
extern "C" float func_001385A8(void *);
extern "C" void func_00203E10(Vector *, float);
extern "C" int func_00238F48();
extern "C" void func_0015BFF8(void *,void *);
void func_0015C288(Obj *obj) {
 char *base = obj->x0;
 void *dest = base+0x63DC;
 float scale = D_0029C700 + (D_0029C704-D_0029C700)*func_001385A8(base+0x40C0);
 float *p=(float *)(base+0x4180);
 float x=p[0],y=p[1],z=p[2],length,inv;
 int bits;
 /* Sony length/reciprocal wrapper, as used by promoted vector helpers. */
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(length),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 Vector v;
 v.x=x*inv; v.y=y*inv; v.z=z*inv;
 float *q=(float *)(*(char **)(base+0x792C)+0x50);
 float dot;
 __asm__ volatile("mula.s %1,%4\n\tmadda.s %2,%5\n\tmadd.s %0,%3,%6" : "=f"(dot) : "f"(v.x),"f"(v.y),"f"(v.z),"f"(q[0]),"f"(q[1]),"f"(q[2]));
 Vector copy=*(Vector *)(base+0x20);
 Input *input=(Input *)(base+0x63E0);
 func_00203E10(&copy,scale*dot);
 int elapsed=func_00238F48()-obj->x8;
 if(input->test()) func_0015BFF8(dest,input);
 else if(elapsed>=31) {
  Input *input=(Input *)(base+0x640C);
  if(input->test()) goto found;
  input=(Input *)(base+0x6400);
  if(input->test()) {
found:
   func_0015BFF8(dest,input);
  } else {
   input=(Input *)(base+0x63F4);
   if(input->test()) func_0015BFF8(dest,input);
  }
 }
}
