/* UNIT 0x001936C0 0x00193778 */
typedef struct Obj { char pad0[0x20]; unsigned int x20,x24,x28,x2C,x30,x34,x38,x3C; char pad40[12]; float x4C,x50,x54,x58; char pad5C[0x11]; unsigned char x6D; char pad6E[0x12]; char x80[0x10004]; int x10084; void *x10088; unsigned char x1008C,x1008D; } Obj;
extern void func_00163970(Obj *, int, void *);
extern char D_00308D40[];
extern void func_00193778(Obj *);
void func_001936C0(Obj *o) {
func_00163970(o,0,o->x80);
o->x4C=0.5f;o->x54=o->x50=0.25f;o->x58=0.0008f;
o->x30=0xFF20FF80;o->x34=0x2020FF80;
o->x20=0x20FFFF80;o->x24=0x20202080;
o->x10088=D_00308D40;o->x1008C=0;o->x1008D=0;o->x10084=0;
func_00193778(o);
o->x6D=1;
}
