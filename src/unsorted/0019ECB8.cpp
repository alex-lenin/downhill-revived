/* UNIT 0x0019ECB8 0x0019ED18 */
struct Obj { char pad[0x488]; int x488; };
struct Global { char pad[0x5B74]; int x5B74; };
extern int D_0029D404;
extern Global D_00398C40;
extern char D_004BD740[];
extern int func_001C2A90(void *);
extern void func_00216F48(float);
extern void func_001C1B48(void *);
static inline Global *global() { return &D_00398C40; }
void func_0019ECB8(Obj *obj) {
    obj->x488 = 5;
    D_0029D404 = 10;
    if (!func_001C2A90(D_004BD740)) func_00216F48(0.6f);
    func_001C1B48(&global()->x5B74);
}
