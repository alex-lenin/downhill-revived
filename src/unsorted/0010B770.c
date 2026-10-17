/* UNIT 0x0010B770 0x0010B8E8 */
typedef struct Target { char pad0[0x11]; unsigned char x11; char pad12[2]; unsigned short x14; char x16; unsigned char x17; } Target;
typedef struct Obj { char pad0[0x74]; Target *x74,*x78; int x7c; Target *x80; char pad84[0x234]; float x2b8; char pad2bc[0xf0]; int x3ac,x3b0; char pad3b4[0x54]; int x408; } Obj;
extern char D_0029EA20[];
extern int func_001177C8(void *,Target *),func_001177D0(void *,Target *);
void func_0010B770(Obj *p) {
 int r; unsigned char limit,other;
 if(p->x80) return;
 if(!p->x74) goto clear;
 p->x3ac=func_001177C8(D_0029EA20,p->x74);
 if(p->x3ac && ((p->x74->x14>>4)&1)) p->x3ac=0;
 r=func_001177D0(D_0029EA20,p->x74); p->x3b0=r;
 if(p->x3ac || r) p->x80=p->x74;
 if(!p->x80) {
 p->x3ac=func_001177C8(D_0029EA20,p->x78);
 if(p->x3ac && ((p->x78->x14>>4)&1)) p->x3ac=0;
 r=func_001177D0(D_0029EA20,p->x78); p->x3b0=r;
 if(p->x3ac || r) p->x80=p->x78;
 }
 if(!p->x80) return;
 if(p->x408>=121) goto clear;
 limit=p->x80->x11;
 if(limit) {
 if(limit<(int)(p->x2b8*100.0f)) {
 other=p->x80->x17;
 if(p->x3b0<other) p->x3b0=other;
 } else p->x3ac=p->x3b0+2;
 }
 return;
clear:
 p->x80=0; p->x3ac=0; p->x3b0=0;
}
