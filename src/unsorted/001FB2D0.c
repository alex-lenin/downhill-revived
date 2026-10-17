/* UNIT 0x001FB2D0 0x001FB468 .sdata=0x29D990:0x29D994 */
typedef struct Owner { char pad[0x1198]; int x1198,x119C; char pad2[0x1260-0x11A0]; int x1260,x1264; char pad3[0x13D9-0x1268]; unsigned char x13D9,x13DA; } Owner;
typedef struct Obj { Owner *x0; int x4; float x8; unsigned char xC; char pad[15]; float x1C; int x20,x24,x28; } Obj;
float D_0029D990 = 0.1f;
extern float func_00236088(float,float,float,float);
extern void func_001FB468(Obj *);
void func_001FB2D0(Obj *p) {
 Owner *q=p->x0; float target,current; int a,b;
 if (!p->xC) {
 p->x8=func_00236088(p->x8,1.0f,0.15f,0.08f);
 if(p->x8>=1.0f) {p->x1C=D_0029D990; p->xC=1;}
 } else {
 if(p->x28==0) {
 if(q->x13D9) a=q->x1260; else a=q->x1198;
 if(a) {target=0.015f; current=p->x1C; goto update;}
 if(q->x13DA) b=q->x1264; else b=q->x119C;
 current=p->x1C;
 if(b) {target=-0.02f; goto update;}
 goto zero;
 } else if(p->x28==1) {
 if(q->x13D9) a=q->x1260; else a=q->x1198;
 if(a) {target=-0.02f; current=p->x1C; goto update;}
 if(q->x13DA) b=q->x1264; else b=q->x119C;
 if(b) {current=p->x1C; target=0.015f; goto update;}
 current=p->x1C; goto zero;
 } else goto decay;
 zero: target=0.0f;
 update: p->x1C=func_00236088(current,target,0.01f,0.006f);
 decay:;
 p->x8=func_00236088(p->x8,0.0f,0.15f,0.08f);
 }
 func_001FB468(p);
}
