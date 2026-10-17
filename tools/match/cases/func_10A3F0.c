struct T { int x0, x4; float x8; };
extern int D_43F348[];  /* .bss; not small data */
extern char D_37EA60[], D_759780[], D_260868[];
extern void func_148560(void *, int);
extern void func_236B28(void *, void *, int, int);
void func_10A3F0(int a, struct T *t) {
    D_43F348[0] = 0;
    func_148560(D_37EA60, 7);
    func_236B28(D_759780, D_260868, a, (int)(t->x8 * 60.0f));
}
