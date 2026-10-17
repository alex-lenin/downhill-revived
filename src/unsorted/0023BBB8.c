/* UNIT 0x0023BBB8 0x0023BC10 */
typedef struct Stream {
    char pad0[0x40];
    int sema;
} Stream;

extern void func_0023B1B0(int);
extern void func_0025A6F0(int);

int func_0023BBB8(Stream *s)
{
    func_0023B1B0(5);
    *(volatile int *)0x1000B420 = 0;
    *(volatile int *)0x1000B410 = 0;
    *(volatile int *)0x1000B430 = 0;
    func_0025A6F0(s->sema);
    return 1;
}
