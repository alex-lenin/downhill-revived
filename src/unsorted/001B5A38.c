/* UNIT 0x001B5A38 0x001B5AB0 */
typedef struct { char pad0[0x20]; int x20; char pad24[0x1C]; int x40; char pad44[0x10]; unsigned char x54, x55; char pad56[0xB]; unsigned char x61; } Obj;
extern int D_0029E10C;
extern char D_004B1F70[];
extern void func_00238B20(void);
extern void func_00238BD8(void);
extern void func_001B5C18(Obj *, void *, int);
extern void func_001B5B40(Obj *);
extern void func_001B55F8(Obj *);
void func_001B5A38(Obj *p) {
    p->x40 = D_0029E10C;
    func_00238B20();
    func_00238BD8();
    p->x55 = 1;
    func_001B5C18(p, D_004B1F70, p->x20);
    if (p->x61) func_001B5B40(p);
    p->x54 = 0;
    func_001B55F8(p);
}
