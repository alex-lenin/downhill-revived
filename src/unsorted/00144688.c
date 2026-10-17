/* UNIT 0x00144688 0x00144720 */
typedef struct { char pad[0x20]; unsigned int x20,x24; char pad28[8]; unsigned int x30,x34; char pad38[0x14]; float x4C,x50,x54,x58; char pad5C[0x2024]; int x2080; } Obj;
extern void func_00163970(Obj *,int,void *);
void func_00144688(Obj *o) {
 func_00163970(o,0,(char *)o+0x80);
 o->x4C=5.0f; o->x50=3.5f; o->x54=1.0f; o->x58=0.0008f;
 o->x30=0xFF20FF80; o->x34=0x2020FF80; o->x20=0x20FFFF80; o->x24=0x20202080;
 o->x2080=0;
}
