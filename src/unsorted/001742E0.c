/* UNIT 0x001742E0 0x00174398 */
typedef struct Obj { char pad0[0x20]; unsigned int x20,x24,x28,x2C,x30,x34,x38,x3C; char pad40[12]; float x4C,x50,x54,x58; char pad5C[0x11]; unsigned char x6D; char pad6E[0x12]; char x80[0x2000]; int x2080; } Obj;
extern void func_00163970(Obj *, int, void *);
void func_001742E0(Obj *o) {
func_00163970(o,0,o->x80);
o->x4C=4.0f;o->x50=2.0f;o->x54=1.0f;o->x58=0.0008f;
o->x20=0x20FFFF80;o->x24=0x20202080;o->x2C=o->x28=0xFFFF2080;
o->x30=0xFF20FF80;o->x34=0x2020FF80;o->x3C=o->x38=0x20FF2080;
o->x2080=0;
}
