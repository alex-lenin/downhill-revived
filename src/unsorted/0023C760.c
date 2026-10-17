/* UNIT 0x0023C760 0x0023C7A8 */
typedef struct Obj { long x0; long x8; long x10; } Obj;
extern char D_00663BA0[];
extern void func_0023BF20(void *, void *);
int func_0023C760(void *unused, Obj *p) {
    long local[4];
    func_0023BF20(D_00663BA0, local);
    p->x8 = local[0];
    p->x10 = local[1];
    return 1;
}
