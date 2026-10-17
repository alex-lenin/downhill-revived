/* UNIT 0x0010AD50 0x0010AF20 */
typedef struct Parent { char pad0[0x78D8]; char x78D8[0xA0]; int x7978; char pad797C[0x10]; int x798C; } Parent;
typedef struct Obj { Parent *x0; char pad4[0x274]; int x278; char pad27C[0x30]; float x2AC; char pad2B0[8]; float x2B8; char pad2BC[0x18]; char x2D4[0xF0]; int x3C4[4],x3D4[9]; int x3F8,x3FC; char pad400[0x20]; int x420; char pad424[0xC]; int x430; char pad434[0xC]; unsigned long x440; } Obj;
typedef struct Global { char pad0[0x10C]; int x10C; } Global;
extern Global D_004B25A0;
extern void func_0010AFB0(Obj *,int);
extern void func_0010AFE0(Obj *,int,int,int);
extern float func_001FAC50(void *);
extern void func_0010B0E8(Obj *),func_00113810(Obj *),func_0010AF20(Obj *);
extern int func_00116FA0(void *),func_00117090(void *),func_00222BD8(int,int);
void func_0010AD50(Obj *p,int a,int b,int c) {
 char *sub; int *first,*second,*cursor; int count;
 if(p->x0) {
 sub=p->x2D4; second=p->x3D4;
 p->x440|=(unsigned long)1<<32;
 func_0010AFB0(p,1);
 func_0010AFE0(p,a,b,c);
 p->x440&=~((unsigned long)1<<39); p->x440&=~((unsigned long)1<<41);
 b=(int)p->x3C4;
 p->x278=0; p->x0->x798C=1; c=3; p->x2AC=1.0f;
 p->x2B8=func_001FAC50(p->x0->x78D8);
 func_0010B0E8(p); func_00113810(p);
 p->x3F8=func_00116FA0(sub); p->x3FC=func_00117090(sub);
 do { if(D_004B25A0.x10C==10) *(int *)b=func_00222BD8(0,100); else *(int *)b=p->x3F8; c--; b+=4; } while(c>=0);
 cursor=second; b=10; count=8; do { if(D_004B25A0.x10C==b) *cursor=func_00222BD8(0,100); else *cursor=p->x3FC; count--; cursor++; } while(count>=0);
 p->x420=p->x0->x7978*2; p->x430=0; func_0010AF20(p);
 }
}
