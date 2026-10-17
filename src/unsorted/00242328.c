/* UNIT 0x00242328 0x00242370 */
typedef struct Obj { int x0; char pad4[0x28]; int x2C; int x30; int x34; int x38; int x3C; int x40; int x44; int x48; int x4C; int x50; int x54; } Obj;
extern void func_00202A70(Obj *);
int func_00242328(Obj *p) {
    func_00202A70(p);
    p->x0 = 0;
    p->x2C = 0;
    p->x34 = 0;
    p->x38 = 0;
    p->x40 = 0;
    p->x54 = 0;
    p->x4C = 0;
    p->x50 = 0;
    return 1;
}
