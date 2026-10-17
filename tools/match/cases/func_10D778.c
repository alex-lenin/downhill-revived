struct G { char pad[0x798c]; int x798c; };
struct A { struct G *x0; char pad4[0x404]; int x408; }; int func_10D778(struct A *a) { int r = 0; if ((a->x0 && a->x0->x798c >= 5) || a->x408 >= 121) r = 1; return r; }
