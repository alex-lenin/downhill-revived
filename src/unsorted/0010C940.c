/* UNIT 0x0010C940 0x0010CAF0 */
struct Rider {char pad[0x4044];float x4044;};
struct Obj {struct Rider *x0;char p4[0x434];int x438;char p43C[12];unsigned long x448;};
struct Input {char p0[0x1A4];int x1A4;char p1A8[8];int x1B0,x1B4;char p1B8[0x20];int x1D8;char p1DC[0x38];int x214,x218;char p21C[0x50];int x26C;char p270[8];int x278,x27C;char p280[0x20];int x2A0;char p2A4[0x150];unsigned char x3F4;char p3F5[2];unsigned char x3F7,x3F8;char p3F9[8];unsigned char x401;};
extern void func_0010C630(struct Obj *,struct Input *);
extern float func_001385A8(void *);
extern int func_00222BD8(int,int),func_00238F60(void);
#define MAKE_SET(a,b,c) static __inline__ void set_##a(struct Input *in,int value) {if(in->a) in->b=value; else {in->c=value;in->b=0;}}
MAKE_SET(x3F4,x26C,x1A4)
MAKE_SET(x3F8,x27C,x1B4)
MAKE_SET(x3F7,x278,x1B0)
MAKE_SET(x401,x2A0,x1D8)
#define SET(a,b,c) set_##a(in,1);
void func_0010C940(struct Obj *o,struct Input *in) {
 int pressed;
 if(!(o->x448&2)) {func_0010C630(o,in);return;}
 if(o->x438>0) {o->x438-=func_00238F60();return;}
 o->x0->x4044=-0.7f;
 SET(x3F4,x26C,x1A4)
 if(func_001385A8((char *)o->x0+0x40C0)>0.01f) {
 pressed=in->x3F8 ? in->x27C : in->x218;
 if(pressed) { SET(x3F8,x27C,x1B4) SET(x401,x2A0,x1D8) }
 else {
 pressed=in->x3F7 ? in->x278 : in->x214;
 if(pressed) { SET(x3F7,x278,x1B0) SET(x401,x2A0,x1D8) }
 else if(func_00222BD8(0,1)) { SET(x3F8,x27C,x1B4) SET(x401,x2A0,x1D8) }
 else { SET(x3F7,x278,x1B0) SET(x401,x2A0,x1D8) }
 }
 }
}
