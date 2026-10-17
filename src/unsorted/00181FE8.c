/* UNIT 0x00181FE8 0x00182070 */
extern void func_00163970(void *, int, void *);
typedef struct { char p[0x4c]; float x4c,x50,x54,x58; char p5c[0x2084-0x5c]; int x2084,x2088,x208c; } Obj;
void func_00181FE8(Obj *o, int unused, int a, int b) {
func_00163970(o,0,(char *)o+0x80);
o->x208c=b; o->x2088=a; o->x4c=2.0f; o->x50=1.25f; o->x54=0.3f; o->x58=0.0008f; o->x2084=0;
}
