/* UNIT 0x00153FE8 0x00154058 */
struct Owner { char pad[0x7ADC]; int x7ADC; };
struct Obj { struct Owner *x0; char pad4[0x24]; void *x28; int x2C; char pad30[0x68]; int x98, x9C; };
extern void func_00152768(struct Obj *);
extern void func_00209060(void *);
extern void func_00130A88(struct Owner *);
void func_00153FE8(struct Obj *p) {
 int state;
 func_00152768(p);
 if (p->x28 && p->x2C == -1) { func_00209060(p->x28); p->x28=0; p->x9C=0; p->x98=0; }
 state=p->x0->x7ADC;
 if (state <= 0) { if (state >= 0) func_00130A88(p->x0); }
}
