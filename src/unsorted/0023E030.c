/* UNIT 0x0023E030 0x0023E0A8 */
typedef struct { int x0; char pad4[0x8573C]; } Entry;
typedef struct { int x0; Entry *x4; volatile int x8,xC; int x10; } Obj;
extern int DIntr(void);
extern void func_0025EEA0(void);
void func_0023E030(Obj *p) {
    DIntr();
    p->x4[p->x8].x0=2;
    p->xC++;
    p->x8=(p->x8+1)%p->x10;
    func_0025EEA0();
}
