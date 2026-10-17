/* UNIT 0x0010B8E8 0x0010B920 */
struct G { char pad[0x792c]; char *x792c; };
struct A { struct G *x0; char pad4[0x70]; int x74; };
extern char D_0029EA20[];
extern void *func_00117828(void *, void *);

void *func_0010B8E8(struct A *a) {
    if (a->x74 == 0) return func_00117828(D_0029EA20, a->x0->x792c + 0x10);
}
