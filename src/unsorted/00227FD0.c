/* UNIT 0x00227FD0 0x00228040 */
typedef struct {
    unsigned long x0_0 : 7;
    unsigned long id : 11;
    unsigned long x0_18 : 21;
    unsigned long flag : 1;
    unsigned long x0_40 : 24;
} Handle;

extern unsigned int D_0029E314;

void func_00227FD0(Handle *h)
{
    /* flag set and id still 0 */
    if ((*(unsigned long *)h & 0x800003ff80UL) == 0x8000000000UL) {
        h->id = D_0029E314++;
    } else {
        h->id = 0x7ff;
    }
}
