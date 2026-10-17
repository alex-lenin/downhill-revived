/* UNIT 0x0020F588 0x0020F5D0 */
struct Entry { char pad[0x18]; unsigned char x18, x19, x1A, x1B; char pad1C[4]; };
extern struct Entry D_004F3570[];
extern void func_0020F420(int, void *);
void func_0020F588(void *arg0, int index, int x18, int x19, int x1A, int x1B) {
    struct Entry *p = &D_004F3570[index];
    p->x18 = x18;
    p->x19 = x19;
    p->x1A = x1A;
    p->x1B = x1B;
    func_0020F420(index, arg0);
}
