/* UNIT 0x00134CC8 0x00134D60 .sdata=0x29C2F0:0x29C2F8 */
typedef struct { float x0[3]; float xC; } Obj;
typedef struct { char pad[0x24]; float x24,x28; char pad2[8]; float x34,x38; } Mat;
int D_0029C2F0 = 1;
int D_0029C2F4 = 0;
extern void func_00134E88(Obj *,float);
static __inline__ float len2(float x,float y) { float s,r; s=x*x+y*y; __asm__ volatile("sqrt.s %0,%1" : "=&f"(r) : "f"(s)); return r; }
void func_00134CC8(Obj *o, Mat *m, float angle) {
 float a,b,c,d,e,f,len,lenB;
 a=o->xC; c=m->x24; d=m->x28;
 len=len2(a,angle);
 a=a/len; b=-angle/len;
 f=d*a+c*b;
 c=c*a-d*b;
 a=f;
 lenB=len2(c,a);
 d=a/lenB; c=c/lenB;
 m->x28=d; m->x24=c; m->x38=c; m->x34=-d;
 if(!D_0029C2F0) func_00134E88(o,0.5f);
}
