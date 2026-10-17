struct G { char pad[0x792c]; char *x792c; };
struct A { struct G *x0; char pad4[0x70]; int x74; };
extern char D_29EA20[];
extern void func_117828(void *, void *);
void func_10B8E8(struct A *a) { if (a->x74 == 0) func_117828(D_29EA20, a->x0->x792c + 0x10); }
