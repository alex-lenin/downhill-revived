/* UNIT 0x0023C420 0x0023C498 */
typedef struct { int x0, x4, x8, xC; } State;
extern State D_00663B40;
extern int func_0023B2C0(void *);
extern void func_0023E000(State *);
extern int func_0023C498(void *);
extern void func_00224058(void);
extern void func_0023C290(void *, int);
void func_0023C420(char *obj) {
    func_0023B2C0(obj + 0x48);
    func_0023E000(&D_00663B40);
    func_0023C498(obj);
    if (D_00663B40.xC != 0) { do { func_00224058(); } while (D_00663B40.xC != 0); }
    func_0023C290(obj, 3);
}
