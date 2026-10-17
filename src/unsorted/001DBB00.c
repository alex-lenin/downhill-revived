/* UNIT 0x001DBB00 0x001DBBB8 */
typedef struct { char p0[0x20]; unsigned int x20,x24,x28,x2C,x30,x34,x38; char p3C[0x10]; float x4C,x50,x54,x58; char p5C[0x2028]; int x2084; } Obj;
extern void func_00163970(Obj *, int, void *);
void func_001DBB00(Obj *o) { func_00163970(o,0,(char*)o+0x80); o->x4C=.4f; o->x50=.2f; o->x54=.3f; o->x58=.0008f; o->x30=0xFF20FF80; o->x34=0x2020FF80; o->x20=0x20FFFF80; o->x24=0x20FF2080; o->x38=0x40400880; o->x28=0xFF202080; o->x2084=0; }
