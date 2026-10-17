/* UNIT 0x001DF638 0x001DF720 */
typedef struct Obj {
char p0[0x8c]; int x8c; char p90[0xf8]; int x188; int x18c; int x190; int x194; int x198; char p19c[0x2c]; int x1c8[10]; int x1f0; int x1f4; char p1f8[0x28]; int x220;
} Obj;
extern void func_002025A0(int);
extern int func_00238F48(void);
extern void func_001DF820(Obj *);
void func_001DF638(Obj *p) {
char *slots;
if (p->x220 || p->x1f4) {
slots = (char *)p->x1c8;
func_002025A0(*(int *)(slots + (p->x18c << 2))); *(int *)(slots + (p->x18c << 2))=0;
func_002025A0(*(int *)(slots + (p->x194 << 2))); *(int *)(slots + (p->x194 << 2))=0;
func_002025A0(*(int *)(slots + (p->x194 << 2))); slots += (p->x194 << 2); *(int *)slots=0;
p->x1f4=0; p->x198=0; p->x1f0=func_00238F48(); p->x220=0;
}
p->x8c=0; p->x188=1;
func_001DF820(p);
p->x190=p->x18c; p->x194=p->x18c;
}
