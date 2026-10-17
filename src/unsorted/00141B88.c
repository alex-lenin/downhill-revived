/* UNIT 0x00141B88 0x00141BE0 */
struct Global { char pad0[0x30]; char x30[0x18]; char x48[0x18]; char x60[4]; };
extern struct Global D_0036B730;
extern void func_00223A98(void *, int, int);
extern void func_00207F80(void *);

static inline void func_00141B88_reset(struct Global *g)
{
    func_00223A98(g->x30, 0, 0);
    func_00223A98(g->x48, 0, 0);
    func_00207F80(g->x60);
}

void func_00141B88(int arg0, int arg1)
{
    if (arg1 == 0xffff && arg0 != 0) {
        func_00141B88_reset(&D_0036B730);
    }
}
